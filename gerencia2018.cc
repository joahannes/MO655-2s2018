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

#include "ns3/netanim-module.h"

//Monitor de fluxo
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"

#include "ns3/gnuplot.h"

// Default Network Topology
//
//		Wifi 10.1.1.0
//            AP     
//  *    *    *    *    *
//  |    |    |    |    |
// s0   s1   a0   s2   s3
// 
// Number of wifi nodes can be increased up to 250


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("projetoGerencia2018");

void ThroughputMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset){

  double tempThroughput = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024);
      dataset.Add((double)Simulator::Now().GetSeconds(), (double)tempThroughput);
  }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&ThroughputMonitor, fmhelper, monitor, dataset);
}

void DelayMonitor (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset1){
  
  double delay = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      //Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      delay = stats->second.delaySum.GetSeconds ();
      dataset1.Add((double)Simulator::Now().GetSeconds(), (double)delay);
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&DelayMonitor, fmhelper, monitor, dataset1);
}

void ImprimeMetricas (FlowMonitorHelper* fmhelper, Ptr<FlowMonitor> monitor){
  double tempThroughput = 0.0;
  monitor->CheckForLostPackets(); 
  std::map<FlowId, FlowMonitor::FlowStats> flowStats = monitor->GetFlowStats();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (fmhelper->GetClassifier());

// .host[82].nic.mac1609_4   TotalLostPackets  0
// scalar VANET.host[82].nic.mac1609_4   DroppedPacketsInMac   0
// scalar VANET.host[82].nic.mac1609_4   TooLittleTime   0
// scalar VANET.host[82].nic.mac1609_4   TimesIntoBackoff  106
// scalar VANET.host[82].nic.mac1609_4   SlotsBackoff  171
// scalar VANET.host[82].nic.mac1609_4   NumInternalContention   0
// scalar VANET.host[82].nic.mac1609_4   totalBusyTime   0.113128999995

 
  std::cout<<"\n"<<std::endl;
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator stats = flowStats.begin (); stats != flowStats.end (); ++stats){ 
      // A tuple: Source-ip, destination-ip, protocol, source-port, destination-port
      Ipv4FlowClassifier::FiveTuple fiveTuple = classifier->FindFlow (stats->first);
      //controle
      std::string flowidhost = "FlowID[" + std::to_string(stats->first) + "]";
      std::cout << flowidhost <<"   Flow   "<< fiveTuple.sourceAddress <<" -----> "<<fiveTuple.destinationAddress<<std::endl;
      std::cout << flowidhost <<"   TxPackets   " << stats->second.txPackets<<std::endl;
      std::cout << flowidhost <<"   RxPackets   " << stats->second.rxPackets<<std::endl;
      std::cout << flowidhost <<"   Duration   " <<stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds()<<std::endl;
      std::cout << flowidhost <<"   LastReceivedPacket(s)   "<< stats->second.timeLastRxPacket.GetSeconds()<<std::endl;
      tempThroughput = (stats->second.rxBytes * 8.0 / (stats->second.timeLastRxPacket.GetSeconds() - stats->second.timeFirstTxPacket.GetSeconds())/1024/1024);
      std::cout << flowidhost <<"   Throughput(Mbps)   "<< tempThroughput<<std::endl;
      std::cout << flowidhost <<"   Delay   " << stats->second.delaySum.GetSeconds () << std::endl;
      std::cout << flowidhost <<"   LostPackets   " << stats->second.lostPackets << std::endl;
      std::cout << flowidhost <<"   Jitter   " << stats->second.jitterSum.GetSeconds () << std::endl;
      //std::cout<<"------------------------------------------"<<std::endl;
    }
  
  //Tempo que será iniciado
  Simulator::Schedule(Seconds(1),&ImprimeMetricas, fmhelper, monitor);
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
  int trafego = 1;


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

//Cria o nó AP para WiFi
  NodeContainer wifiApNode;
  wifiApNode.Create (apWifi);

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
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;
  //Cria endereço para os Clientes
  address.SetBase ("192.168.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfacesAp = address.Assign (apDevices);
  Ipv4InterfaceContainer interfacesWifi = address.Assign (staDevices);

//Aplicacao
for (uint32_t i = 0; i < wifiStaNodes.GetN(); i++){

  uint16_t port = 1000;
  uint16_t m_port = port * i + 1000; //Para alcançar o nó ZERO quando i = 0

  OnOffHelper onoff ("ns3::UdpSocketFactory", Address(InetSocketAddress(interfacesAp.GetAddress(0), m_port)));
  onoff.SetAttribute ("Remote",  AddressValue(InetSocketAddress(interfacesWifi.GetAddress(i), m_port)));
  onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
  onoff.SetAttribute ("DataRate", DataRateValue ( DataRate (117760.0 / 1)));
  onoff.SetAttribute ("PacketSize", UintegerValue (1472));

  ApplicationContainer app = onoff.Install(wifiApNode.Get(0));
  app.Start(Seconds (1.0));
  app.Stop(Seconds (simTime));

  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress(interfacesWifi.GetAddress(i), m_port));

  ApplicationContainer sinkApp = sink.Install(wifiStaNodes.Get(i));
  sinkApp.Start(Seconds (2.0));
  sinkApp.Stop(Seconds (simTime));
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
  AnimationInterface anim ("Gerencia2018_anim.xml");
//Cor e Descrição para AP
  for (uint32_t i = 0; i < wifiApNode.GetN (); ++i){
      anim.UpdateNodeDescription (wifiApNode.Get (i), "AP");
      anim.UpdateNodeColor (wifiApNode.Get (i), 0, 255, 0);
  }
  anim.EnablePacketMetadata ();

  Simulator::Stop (Seconds (17.0));

  std::string tipo = "resultados/";

  //Throughput
    std::string vazao = tipo + "Vazao";
    std::string graphicsFileName        = "Vazao.eps";
    std::string plotFileName            = vazao + ".plt";
    std::string plotTitle               = "Fluxo vs Vazao";
    std::string dataTitle               = "Vazao";

    Gnuplot gnuplot (graphicsFileName);
    gnuplot.SetTitle (plotTitle);
    gnuplot.SetTerminal ("eps");
    gnuplot.SetLegend ("Fluxo", "Vazao (Kbps)");

    Gnuplot2dDataset dataset;
    dataset.SetTitle (dataTitle);
    dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);

  //Delay
    std::string delay = tipo + "Atraso";
    std::string graphicsFileName1        = "Atraso.eps";
    std::string plotFileName1            = delay + ".plt";
    std::string plotTitle1               = "Flow vs Atraso";
    std::string dataTitle1               = "Atraso";

    Gnuplot gnuplot1 (graphicsFileName1);
    gnuplot1.SetTitle (plotTitle1);
    gnuplot1.SetTerminal ("eps");
    gnuplot1.SetLegend ("Flow", "Atraso");

    Gnuplot2dDataset dataset1;
    dataset1.SetTitle (dataTitle1);
    dataset1.SetStyle (Gnuplot2dDataset::LINES_POINTS);

//Chama a captura de fluxo
    ThroughputMonitor (&fmhelper, monitor, dataset);
    DelayMonitor (&fmhelper, monitor, dataset1);
  
  Simulator::Run ();

//Imprime métricas no Terminal
  if (verbose)
  {
  	ImprimeMetricas (&fmhelper, monitor);
  }
  //Throughput
    gnuplot.AddDataset (dataset);
    std::ofstream plotFile (plotFileName.c_str()); // Abre o arquivo.
    gnuplot.GenerateOutput (plotFile);    //Escreve no arquivo.
    plotFile.close ();        // fecha o arquivo.
  //Delay
    gnuplot1.AddDataset (dataset1);
    std::ofstream plotFile1 (plotFileName1.c_str()); // Abre o arquivo.
    gnuplot1.GenerateOutput (plotFile1);    //Escreve no arquivo.
    plotFile1.close ();        // fecha o arquivo.

//FlowMonitor
  monitor->SerializeToXmlFile("resultados/fluxo_projeto.xml", true, true);


  Simulator::Destroy ();
  return 0;
}
