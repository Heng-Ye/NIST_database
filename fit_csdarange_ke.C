#include <iostream>
#include <fstream>
#include <string>
#include "TGraphSmooth.h" 

//#include "TSpline.h"

using namespace std;


const double m_proton=0.938272046; //GeV/c2

double p2ke(double p) {
 double ke=m_proton*(-1.+sqrt(1.+(p*p)/(m_proton*m_proton)));
 return ke;
}


void fit_csdarange_ke(){

  vector<double> data;
  vector<double> ke;
  vector<double> csda_range;
  vector<double> ex;
  string line;

  int n=2; //2 columns
  double buffer;
  ifstream f_in("proton_csda_ke.dat");
  while (f_in>>buffer) { data.push_back(buffer); }
  f_in.close();
  
  for (int k=0; k<data.size(); k+=n) {
    //ke.push_back(data[k]);
    ke.push_back(1000.*data[k]);
    csda_range.push_back(data[k+1]);
    ex.push_back(0);	
  } 

  for (int k=0; k<ke.size(); k++) {
    std::cout<<ke[k]<<" "<<csda_range[k]<<std::endl;
  }


   //settting for figures ----------------------------------------------//
   gStyle->SetOptStat(0); //no statistics box
   gStyle->SetCanvasColor(0);
   gStyle->SetPadColor(0);
   gStyle->SetFrameFillColor(0); //Set the frame color to white
   gStyle->SetTitleFillColor(0);
   gStyle->SetStatColor(0);
   gStyle->SetCanvasBorderMode(0); // -1=down, 0=no border, 1=up
   gStyle->SetFrameBorderSize(3);
   gStyle->SetTextSize(1.1);

   gStyle->SetLabelSize(0.05,"xy");
   gStyle->SetTitleSize(0.06,"xy");
   gStyle->SetTitleSize(0.07,"t");

   gStyle->SetLineWidth(2);
   gStyle->SetFrameLineWidth(2);

   gStyle->SetTitleOffset(.9,"xy");
   //gStyle->SetTitleOffset(1.2,"y");
   gStyle->SetTitleOffset(1.2,"t");
   //float global_btmargin=0.15;

   gStyle->SetPadBottomMargin(0.14);
   gStyle->SetPadLeftMargin(0.13);
   //gStyle->SetPadRightMargin(0.1);
   gStyle->SetPaintTextFormat("5.2f");
   Float_t lt_size=0.09;
   //-------------------------------------------------------------------//



  TCanvas *c1 = new TCanvas("c1"," ",200,10,1000,900);
  //c1->cd(1)->SetFillColor(38);
  
  TGraph *csda_range_vs_ke=new TGraph(ke.size(), &ke[0], &csda_range[0]);
  TGraph *csda_range_vs_ke_sm=new TGraph(ke.size(), &ke[0], &csda_range[0]);
  
  //csda_range_vs_ke->GetXaxis()->SetTitle("Proton Kinetic Energy [GeV]");
  csda_range_vs_ke->GetXaxis()->SetTitle("Proton Kinetic Energy [MeV]");
  csda_range_vs_ke->GetYaxis()->SetTitle("CSDA Range [cm]");
  csda_range_vs_ke->SetMarkerSize(2.4);
  csda_range_vs_ke->SetMarkerStyle(20);
  csda_range_vs_ke->SetMarkerColor(2);
  csda_range_vs_ke->Draw("ap");

  TGraphSmooth *gs = new TGraphSmooth("supsmu");
  //csda_range_vs_ke_sm= gs->Approx(csda_range_vs_ke_sm,"linear",40);
  csda_range_vs_ke_sm= gs->Approx(csda_range_vs_ke_sm,"linear",1000);
  csda_range_vs_ke_sm->SetLineColor(1);
  csda_range_vs_ke_sm->Draw("same");

  //get numerical values from TGraphSmooth function
  std::cout<<"1 GeV/c proton has KE:"<<p2ke(1.)<<" GeV"<<std::endl;

  double ke_val=p2ke(1.);
  double csda_val=csda_range_vs_ke_sm->Eval(ke_val);
  std::cout<<"ke="<<ke_val<<" csda_val="<<csda_val<<std::endl;

  TLegend *leg=new TLegend(0.203673,0.645914,0.573456,0.83917);
  leg->SetLineColor(0);
  TLegendEntry* ll[2];
  ll[0]=leg->AddEntry((TObject*)csda_range_vs_ke,Form("%s","NIST Data Base"),"p");
  ll[1]=leg->AddEntry((TObject*)csda_range_vs_ke_sm,Form("%s","Friedman's Super Smoother"),"l");
  ll[0]->SetTextColor(2);
  ll[1]->SetTextColor(1);
  //leg->SetNColumns(2);
  leg->Draw();


  //----
  TCanvas *c2 = new TCanvas("c2"," ",200,10,1200,800);
  
  TGraph *ke_vs_csda_range=new TGraph(ke.size(), &csda_range[0], &ke[0]);
  TGraph *ke_vs_csda_range_sm=new TGraph(ke.size(), &csda_range[0], &ke[0]);
  
  ke_vs_csda_range->GetYaxis()->SetTitle("Proton Kinetic Energy [MeV]");
  //ke_vs_csda_range->GetYaxis()->SetTitle("Proton Kinetic Energy [GeV]");
  ke_vs_csda_range->GetXaxis()->SetTitle("CSDA Range [cm]");
  ke_vs_csda_range->SetMarkerSize(2.4);
  ke_vs_csda_range->SetMarkerStyle(20);
  ke_vs_csda_range->SetMarkerColor(2);
  ke_vs_csda_range->Draw("ap");

  TGraphSmooth *ggs = new TGraphSmooth("supsmu");
  //csda_range_vs_ke_sm= gs->Approx(csda_range_vs_ke_sm,"linear",40);
  ke_vs_csda_range_sm= ggs->Approx(ke_vs_csda_range_sm,"linear",40000000);
  ke_vs_csda_range_sm->SetLineColor(1);
  ke_vs_csda_range_sm->Draw("same");

  //get numerical values from TGraphSmooth function
  double csda_val2=92.5229;
  double ke_val2=ke_vs_csda_range_sm->Eval(csda_val2);
  std::cout<<"csda="<<csda_val2<<" ke_val="<<ke_val2<<std::endl;

  //Save results
  csda_range_vs_ke->SetName("csda_range_vs_ke");
  csda_range_vs_ke_sm->SetName("csda_range_vs_ke_sm");
  ke_vs_csda_range->SetName("ke_vs_csda_range");
  ke_vs_csda_range_sm->SetName("ke_vs_csda_range_sm");
  //TFile *fout=new TFile("proton_ke_csda_converter.root","create");
  TFile *fout=new TFile("proton_ke_inMeV_csda_converter.root","create");
    csda_range_vs_ke->Write();
    csda_range_vs_ke_sm->Write();
    ke_vs_csda_range->Write();
    ke_vs_csda_range_sm->Write();
 fout->Close();



  //ke_vs_csda_range->GetXaxis()->SetRangeUser(0.,5);


  //get numerical values from TGraphSmooth function
  //std::cout<<"1 GeV/c proton has KE:"<<p2ke(1.)<<" GeV"<<std::endl;
  //double ke_val=p2ke(1.);
  //double csda_val=csda_range_vs_ke_sm->Eval(ke_val);
  //std::cout<<"ke="<<ke_val<<" csda_val="<<csda_val<<std::endl;










  //Friedman's SuperSmoother



// Smooth the (x, y) values by Friedman's ``super smoother''.
//
// Arguments:
// grin: graph for smoothing
//
// span: the fraction of the observations in the span of the running lines
//        smoother, or 0 to choose this by leave-one-out cross-validation.
// bass: controls the smoothness of the fitted curve.
//        Values of up to 10 indicate increasing smoothness.
// isPeriodic: if TRUE, the x values are assumed to be in [0, 1]
//        and of period 1.
// w:     case weights
//
// Details:
// supsmu is a running lines smoother which chooses between three spans for
// the lines. The running lines smoothers are symmetric, with k/2 data points
// each side of the predicted point, and values of k as 0.5 * n, 0.2 * n and
// 0.05 * n, where n is the number of data points. If span is specified,
// a single smoother with span span * n is used.
//
// The best of the three smoothers is chosen by cross-validation for each
// prediction. The best spans are then smoothed by a running lines smoother
// and the final prediction chosen by linear interpolation.
//
// The FORTRAN code says: ``For small samples (n < 40) or if there are
// substantial serial correlations between observations close in x - value,
// then a prespecified fixed span smoother (span > 0) should be used.
// Reasonable span values are 0.2 to 0.4.''
//
// References:
// Friedman, J. H. (1984) SMART User's Guide.
//           Laboratory for Computational Statistics,
//           Stanford University Technical Report No. 1.
//
// Friedman, J. H. (1984) A variable span scatterplot smoother.
//           Laboratory for Computational Statistics,
//           Stanford University Technical Report No. 5.



  //Smooth Regression,

  //TSpline3 *s3 = new TSpline3(“s3”,csda_range_vs_ke->GetX(),csda_range_vs_mom->GetY(),csda_range_vs_mom->GetN());
  //s3->SetLineColor(kBlue);
  //s3->Draw(“l same”);
  



/*
  //fit after 2 GeV and ended at 4.5 GeV
Minimizer is Linear
Chi2                      =        35780
NDf                       =          129
p0                        =      2.25674   +/-   1.84071     
p1                        =      -105.71   +/-   4.88304     
p2                        =      258.084   +/-   1.37981   

*/


}
