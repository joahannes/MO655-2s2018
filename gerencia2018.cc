/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Projeto: Rede 802.11 
 * Disciplina: MO655 - Gerencia de Redes de Computadores - 2018
 * Professor: Edmundo Madeira
 * Autores: Joahannes Costa e Wellington Lobato Jr.
 * Email: joahannes, wellington{@lrc.ic.unicamp.br}
 * Laboratório de Redes de Computadores - LRC
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
//Monitor de fluxo
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/random-variable-stream.h"

// Topologia da rede
// 
//    10.1.1.0
// r0 *--------* AP
//                    192.168.0.0
//               *    *    *    *    *  
//               |    |    |    |    |
//               s0   s1   a0   s2   s3
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("projetoGerencia2018");

//Mobilidade para os dispositivos moveis
void AtualizaVelocidade(Ptr<Node> node) {
    Ptr<UniformRandomVariable> rvar = CreateObject<UniformRandomVariable>();
    Ptr <ConstantVelocityMobilityModel> mobility = node -> GetObject<ConstantVelocityMobilityModel>();
    
    double velocidade = rvar->GetValue(2, 3); // 7.2 a 10.8 km/h
    mobility->SetVelocity (Vector (velocidade, 0.0, 0.0));

    Simulator::Schedule (Seconds (0.5), AtualizaVelocidade, node);
}

void ImprimeMetricas (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, int trafego, int clientes){
	double totalThroughput = 0.0;
	monitor->CheckForLostPackets();
	std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats)
	{
		Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
		std::string flowidhost = "FlowID[" + std::to_string(stats->first) + "]";

		if(trafego == 0)
			std::cout << flowidhost <<"   Trafego   UDP" << std::endl;
		else if(trafego == 1)
			std::cout << flowidhost <<"   Trafego   TCP" << std::endl;
		else if(trafego == 2)
			std::cout << flowidhost <<"   Trafego   UDP/TCP" << std::endl;

		std::cout << flowidhost <<"   Clientes   " << clientes << std::endl; 
		std::cout << flowidhost <<"   Flow   "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
		std::cout << flowidhost <<"   Duration   " <<stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()<<std::endl;
		totalThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024/1024);
		std::cout << flowidhost <<"   Throughput(Mbps)   "<< totalThroughput <<std::endl;
		std::cout << flowidhost <<"   Delay   " << stats->second.delaySum.GetSeconds () / stats->second.rxPackets << std::endl;
		std::cout << flowidhost <<"   LostPackets   " << stats->second.lostPackets << std::endl;
	}
	Simulator::Schedule(Seconds(1),&ImprimeMetricas, fmhelper, monitor, trafego, clientes);
}


int main (int argc, char *argv[])
{
  uint16_t seed = 1;
  bool verbose = true; //Verbose
  uint32_t nWifi = 1; //Quantidade de nós WiFi
  uint32_t apWifi = 1; //Quantidade de APs
  bool mobilidade = false; //Mobilidade
  double distance = 5.0; //Distancia entre os nós
  double simTime = 60.0; //Tempo de simulação
  int trafego = 0;

  CommandLine cmd;
  cmd.AddValue ("seed", "Semente de simulação", seed);
  cmd.AddValue ("nWifi", "Número de clientes", nWifi);
  cmd.AddValue ("trafego", "(0) UDP, (1) TCP ou (2) para ambos", trafego);
  cmd.AddValue ("mobilidade", "(0) Sem mobilidade e (1) Com mobilidade", mobilidade);
  cmd.Parse (argc,argv);

//Seed
  RngSeedManager::SetSeed(12);
  RngSeedManager::SetRun(seed);
  Ptr<UniformRandomVariable> u = CreateObject<UniformRandomVariable> ();

  if (nWifi > 250)
  {
  	std::cout << "Muitos nós wifi criados. Nosso limite é 250." << std::endl;
  	return 1;
  }

//Cria apenas nós WiFi
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

//Cria no AP
  NodeContainer wifiApNode;
  wifiApNode.Create (apWifi);

//Cria Router
  NodeContainer csmaNodes;
  csmaNodes.Add(wifiApNode.Get(0));
  csmaNodes.Create (1);

//Cria conexao entre Router e AP
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

//Cria os nós WiFi e os interliga por meio de um canal
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

//Algoritmo de controle de de taxa, que neste caso e o AARF
  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);

  WifiMacHelper mac;

//SSID do AP
  Ssid ssid = Ssid ("Gerencia2018");
  //Clientes
  mac.SetType ("ns3::StaWifiMac", "Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));
  NetDeviceContainer staDevices = wifi.Install (phy, mac, wifiStaNodes);
  //AP
  mac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
  NetDeviceContainer apDevices = wifi.Install (phy, mac, wifiApNode);

//Instancia a Mobilidade
  //AP_node
  Ptr<ListPositionAllocator> positionAlloc0 = CreateObject<ListPositionAllocator> ();
  positionAlloc0->Add (Vector(75, 75, 0));

  MobilityHelper mobilityAp;
  mobilityAp.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityAp.SetPositionAllocator(positionAlloc0);
  mobilityAp.Install(wifiApNode);

  //Router
  Ptr<ListPositionAllocator> positionAllocRouter = CreateObject<ListPositionAllocator> ();
  positionAllocRouter->Add (Vector(0, 0, 0));

  MobilityHelper router;
  router.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  router.SetPositionAllocator(positionAllocRouter);
  router.Install(csmaNodes.Get(1));

  //WiFi_nodes
  if (mobilidade == false)
  { 
    MobilityHelper mobilityWifi;
    mobilityWifi.SetPositionAllocator ("ns3::GridPositionAllocator",
                             "MinX", DoubleValue (75.0),
                             "MinY", DoubleValue (75.0),
                             "DeltaX", DoubleValue (distance), //Distancia entre os nos em X
                             "DeltaY", DoubleValue (distance), //Distancia entre os nos em Y
                             "GridWidth", UintegerValue (6),
                             "LayoutType", StringValue ("RowFirst"));
    mobilityWifi.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
    mobilityWifi.Install (wifiStaNodes);
  }
  else
  {
  	MobilityHelper mobilityWifi;	  
  	mobilityWifi.SetPositionAllocator ("ns3::GridPositionAllocator",
                              "MinX", DoubleValue (75.0),
                              "MinY", DoubleValue (75.0),
                              "DeltaX", DoubleValue (distance), //Distancia entre os nos em X
                              "DeltaY", DoubleValue (distance), //Distancia entre os nos em Y
                              "GridWidth", UintegerValue (6),
                              "LayoutType", StringValue ("RowFirst")); 
    mobilityWifi.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
    mobilityWifi.Install (wifiStaNodes);

    //Velocidade
    for (uint16_t i = 0; i < nWifi; i++)
    {
      AtualizaVelocidade (wifiStaNodes.Get(i));
    }
  }

//Pilha de Internet
  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  //Cria endereco para CSMA
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces = address.Assign (csmaDevices);

  //Cria endereco para os Clientes
  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesAp = address.Assign (apDevices);
  Ipv4InterfaceContainer interfacesWifi = address.Assign (staDevices);

  if(trafego == 0)
  {
    //Aplicacao UDP
    uint16_t port = 9;
    for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++)
    {
      uint16_t m_port = port + i;

      OnOffHelper onoff ("ns3::UdpSocketFactory", Address(InetSocketAddress(csmaInterfaces.GetAddress(1), m_port)));
      onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), m_port)));
      onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
      onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
      onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("1Mbps")));
      onoff.SetAttribute ("PacketSize", UintegerValue (450)); // +34 header

      ApplicationContainer server = onoff.Install(csmaNodes.Get(1));
      server.Start(Seconds (1.0));
      server.Stop(Seconds (simTime));

      PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), m_port));

      ApplicationContainer client = sink.Install(wifiStaNodes.Get(i));
      client.Start(Seconds (2.0));
      client.Stop(Seconds (simTime));
    }
  }
  else if(trafego == 1)
  {
        //Aplicacao TCP
    uint16_t port = 8080;
    for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++)
    {
      uint16_t m_port = port + i;

      OnOffHelper onoff ("ns3::TcpSocketFactory", Address(InetSocketAddress(csmaInterfaces.GetAddress(1), m_port)));
      onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), m_port)));
      onoff.SetAttribute ("OnTime", StringValue("ns3::NormalRandomVariable[Mean=5.|Variance=1.|Bound=10.]"));
      onoff.SetAttribute ("OffTime", StringValue("ns3::NormalRandomVariable[Mean=7.|Variance=1.|Bound=10.]"));
      onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("1Mbps")));
      onoff.SetAttribute ("PacketSize", UintegerValue (1426)); // +60 header

      ApplicationContainer server = onoff.Install(csmaNodes.Get(1));
      server.Start (Seconds (1.0));
      server.Stop (Seconds (simTime));

      PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), m_port));

      ApplicationContainer client = sink.Install(wifiStaNodes.Get(i));
      client.Start(Seconds (2.0));
      client.Stop(Seconds (simTime));
      
    }

  }
  else if(trafego == 2)
  {
  	//Aplicação UDP e TCP
  	for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++)
  	{
  		uint32_t udp_port = 9;
  		uint32_t tcp_port = 8080;

  		if(i % 2 == 0)
  		{
  			//Aplicacao UDP
			OnOffHelper onoff ("ns3::UdpSocketFactory", Address(InetSocketAddress(csmaInterfaces.GetAddress(1), udp_port+i)));
			onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), udp_port+i)));
			onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
			onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
			onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("1Mbps")));
			onoff.SetAttribute ("PacketSize", UintegerValue (478)); // +34 header

			ApplicationContainer server = onoff.Install(csmaNodes.Get(1));
			server.Start(Seconds (1.0));
			server.Stop(Seconds (simTime));

			PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), udp_port+i));

			ApplicationContainer client = sink.Install(wifiStaNodes.Get(i));
			client.Start(Seconds (2.0));
			client.Stop(Seconds (simTime));
		}
  		else
  		{
			//Aplicacao TCP
			OnOffHelper onoff ("ns3::TcpSocketFactory", Address(InetSocketAddress(csmaInterfaces.GetAddress(1), tcp_port+i)));
			onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), tcp_port+i)));
			onoff.SetAttribute ("OnTime", StringValue("ns3::NormalRandomVariable[Mean=5.|Variance=1.|Bound=10.]"));
			onoff.SetAttribute ("OffTime", StringValue("ns3::NormalRandomVariable[Mean=7.|Variance=1.|Bound=10.]"));
			onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("1Mbps")));
			onoff.SetAttribute ("PacketSize", UintegerValue (1440)); // +60 header

			ApplicationContainer server = onoff.Install(csmaNodes.Get(1));
			server.Start (Seconds (1.0));
			server.Stop (Seconds (simTime));

			PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), tcp_port+i));

			ApplicationContainer client = sink.Install(wifiStaNodes.Get(i));
			client.Start(Seconds (2.0));
			client.Stop(Seconds (simTime));
  		}
  	}
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//Monitor de fluxo
  Ptr<FlowMonitor> monitor;
  FlowMonitorHelper fmhelper;
  monitor = fmhelper.InstallAll();

  // //NetAnim
  //   if (trafego == 0)
  //   {
  // 	 AnimationInterface anim ("Gerencia2018_UDP_anim.xml");
  // 	 anim.UpdateNodeDescription (wifiApNode.Get (0), "AP");
  // 	 anim.UpdateNodeColor (wifiApNode.Get (0), 0, 255, 0);
  // 	 anim.UpdateNodeDescription (csmaNodes.Get (1), "ROUTER");
  // 	 anim.UpdateNodeColor (csmaNodes.Get (1), 0, 0, 0);
  // 	 //anim.EnablePacketMetadata ();
  //   }

  Simulator::Stop (Seconds (simTime));
  Simulator::Run ();

//Imprime métricas no Terminal
  if (verbose)
  {
    ImprimeMetricas (&fmhelper, monitor, trafego, nWifi);
  }

  Simulator::Destroy ();
  return 0;
}
