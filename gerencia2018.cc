/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Projeto: Rede 802.11 
 * Disciplina: MO655 - Gerencia de Redes de Computadores - 2018
 * Professor: Edmundo Madeira
 * Autores: Joahannes Costa e Wellington Lobato Jr.
 * Email: joahannes, wellington{@lrc.ic.unicamp.br}
 *
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

#include "ns3/gnuplot.h"

// Default Network Topology
//
//     192.168.0.0      AP   10.1.1.0
//  *    *    *    *    *---------------*r0
//  |    |    |    |    |
// s0   s1   a0   s2   s3
// 
// Number of wifi nodes can be increased up to 250

// Aplicações UDP, TCP e Ambas - OK
// Porcentagem de nós ativos
// 


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("projetoGerencia2018");


void ImprimeMetricas (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, int trafego){
  double tempThroughput = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  //std::cout<<"\n"<<std::endl;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      // A tuple: Source-ip, destination-ip, protocol, source-port, destination-port
      Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      //controle
      //std::cout<<std::endl;
      std::string flowidhost = "FlowID[" + std::to_string(stats->first) + "]";
      // if(trafego == 0){
      //   std::cout << flowidhost <<"   Trafego   UDP" << std::endl;
      // }else if(trafego == 1){
      //   std::cout << flowidhost <<"   Trafego   TCP" << std::endl;
      // }else if(trafego == 2){
      //   std::cout << flowidhost <<"   Trafego   UDP/TCP" << std::endl;
      // }
      std::cout << flowidhost <<"   Flow   "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
      //std::cout << flowidhost <<"   TxPackets   " << stats->second.txPackets<<std::endl;
      //std::cout << flowidhost <<"   RxPackets   " << stats->second.rxPackets<<std::endl;
      std::cout << flowidhost <<"   Duration   " <<stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()<<std::endl;
      //std::cout << flowidhost <<"   LastReceivedPacket(s)   "<< stats->second.timeLastRxPacket.GetSeconds()<<std::endl;
      tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024/1024);
      std::cout << flowidhost <<"   Throughput(Mbps)   "<< tempThroughput<<std::endl;
      std::cout << flowidhost <<"   Delay   " << stats->second.delaySum.GetSeconds () << std::endl;
      std::cout << flowidhost <<"   LostPackets   " << stats->second.lostPackets << std::endl;
      //std::cout << flowidhost <<"   Jitter   " << stats->second.jitterSum.GetSeconds () << std::endl;
      //std::cout<<"------------------------------------------"<<std::endl;
    }
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&ImprimeMetricas, fmhelper, monitor, trafego);
}


int main (int argc, char *argv[])
{
  uint16_t seed = 1;
  bool verbose = true; //Verbose
  uint32_t nWifi = 5; //Quantidade de nós WiFi
  uint32_t apWifi = 1; //Quantidade de APs
  bool tracing = true; //PCAP
  double distance = 5.0; //Distancia entre os nós
  double simTime = 20.0; //Tempo de simulação
  int trafego = 0;


  CommandLine cmd;
  cmd.AddValue ("seed", "Semente de simulação", seed);
  cmd.AddValue ("nWifi", "Número de nós CLIENTES", nWifi);
  cmd.AddValue ("nApWifi", "Número de APs", apWifi);
  cmd.AddValue ("verbose", "Gerar log das transmissoes", verbose);
  cmd.AddValue ("tracing", "Ativar rastreio PCAP", tracing);
  cmd.AddValue ("trafego", "(0) UDP, (1) TCP ou (2) para ambos", trafego);
  cmd.Parse (argc,argv);

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


//Cria P2P do Router e AP
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

//Cria os nós WiFi e os interliga por meio de um canal
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

//Algoritmo de controle de de taxa, que neste caso é o AARF
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
  positionAlloc0->Add (Vector(0, 0, 0));

  MobilityHelper mobilityAp;
  mobilityAp.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityAp.SetPositionAllocator(positionAlloc0);
  mobilityAp.Install(wifiApNode);

  Ptr<ListPositionAllocator> positionAllocRouter = CreateObject<ListPositionAllocator> ();
  positionAllocRouter->Add (Vector(0, -10, 0));

  MobilityHelper router;
  router.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  router.SetPositionAllocator(positionAllocRouter);
  router.Install(csmaNodes.Get(1));

  //WiFi_nodes
  Ptr<ListPositionAllocator> positionAlloc1 = CreateObject<ListPositionAllocator> ();
  for (uint16_t i = 0; i < wifiStaNodes.GetN(); i++){
     positionAlloc1->Add (Vector(distance * i + 10, i * 10, 0));
    }

  MobilityHelper mobilityWifi;
  mobilityWifi.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobilityWifi.SetPositionAllocator(positionAlloc1);
  mobilityWifi.Install(wifiStaNodes);

//Pilha de Internet
  InternetStackHelper stack;
  stack.Install (csmaNodes); //Ja inclue o APnode
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces = address.Assign (csmaDevices);

  //Cria endereço para os Clientes
  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesAp = address.Assign (apDevices);
  Ipv4InterfaceContainer interfacesWifi = address.Assign (staDevices);

  //UDP -> 0
  //TCP -> 1
  //AMBOS -> 2
  if(trafego == 0)
  {
    //Aplicacao UDP
    for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++){

      uint16_t port = 1000;
      uint16_t m_port = port * i + 1000; //Para alcançar o nó ZERO quando i = 0

      OnOffHelper onoff ("ns3::UdpSocketFactory", Address(InetSocketAddress(csmaInterfaces.GetAddress(1), m_port)));
      onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), m_port)));
      onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
      onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
      onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("512kbps")));
      onoff.SetAttribute ("PacketSize", UintegerValue (512));

      ApplicationContainer app = onoff.Install(csmaNodes.Get(1));
      app.Start(Seconds (1.0));
      app.Stop(Seconds (simTime));

      PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), m_port));

      ApplicationContainer sinkApp = sink.Install(wifiStaNodes.Get(i));
      sinkApp.Start(Seconds (2.0));
      sinkApp.Stop(Seconds (simTime));
    }
  }
  else if(trafego == 1){
    //Aplicacao TCP
    for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++){

      uint16_t port = 1000;
      uint16_t m_port = port * i + 1000; //Para alcançar o nó ZERO quando i = 0

      OnOffHelper onoff ("ns3::TcpSocketFactory", Address(InetSocketAddress(interfacesAp.GetAddress(0), m_port)));
      onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), m_port)));
      onoff.SetAttribute ("OnTime", StringValue ("ns3::NormalRandomVariable[Mean=5.|Variance=1.|Bound=10.]"));
      onoff.SetAttribute ("OffTime", StringValue ("ns3::NormalRandomVariable[Mean=7.|Variance=1.|Bound=10.]"));
      onoff.SetAttribute ("DataRate", DataRateValue ( DataRate ("512kbps")));
      onoff.SetAttribute ("PacketSize", UintegerValue (1500));

      ApplicationContainer app = onoff.Install(wifiApNode.Get(0));
      app.Start(Seconds (1.0));
      app.Stop(Seconds (simTime));

      PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), m_port));

      ApplicationContainer sinkApp = sink.Install(wifiStaNodes.Get(i));
      sinkApp.Start(Seconds (2.0));
      sinkApp.Stop(Seconds (simTime));
    }

  }else if(trafego == 2){
    return 1;
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//Monitor de fluxo
  Ptr<FlowMonitor> monitor;
  FlowMonitorHelper fmhelper;
  monitor = fmhelper.InstallAll();

  if (tracing == true)
    {
      phy.EnablePcap ("resultados/gerencia_ap_trace", apDevices.Get (0));
    }

//NetAnim
  if(trafego == 0)
  {
    AnimationInterface anim ("resultados/Gerencia2018_anim_UDP.xml");
    anim.UpdateNodeDescription (wifiApNode.Get (0), "AP");
    anim.UpdateNodeColor (wifiApNode.Get (0), 0, 255, 0);
    anim.UpdateNodeDescription (csmaNodes.Get (1), "ROUTER");
    anim.UpdateNodeColor (csmaNodes.Get (1), 0, 0, 0);
    anim.EnablePacketMetadata ();    
  }
  else if(trafego == 1)
  {

  }

  Simulator::Stop (Seconds (simTime));
  Simulator::Run ();

//Imprime métricas no Terminal
  if (verbose)
  {
  	ImprimeMetricas (&fmhelper, monitor, trafego);
  }

//FlowMonitor
  //monitor->SerializeToXmlFile("resultados/fluxo_projeto.xml", true, true);

  Simulator::Destroy ();
  return 0;
}
