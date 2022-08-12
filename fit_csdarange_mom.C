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

void fit_csdarange_mom(){

  vector<double> data;
  vector<double> mom;
  vector<double> csda_range;
  vector<double> ex;
  string line;

  int n=2; //2 columns
  double buffer;
  ifstream f_in("proton_csda_mom.dat");
  while (f_in>>buffer) { data.push_back(buffer); }
  f_in.close();
  
  for (int k=0; k<data.size(); k+=n) {
    mom.push_back(data[k]);
    csda_range.push_back(data[k+1]);
    ex.push_back(0);	
  } 

  for (int k=0; k<mom.size(); k++) {
    std::cout<<mom[k]<<" "<<csda_range[k]<<std::endl;
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

   gStyle->SetTitleOffset(.7,"xy");
   //gStyle->SetTitleOffset(1.2,"y");
   gStyle->SetTitleOffset(1.2,"t");
   //float global_btmargin=0.15;

   //gStyle->SetPadBottomMargin(0.15);
   //gStyle->SetPadLeftMargin(0.16);
   //gStyle->SetPadRightMargin(0.1);
   gStyle->SetPaintTextFormat("5.2f");
   Float_t lt_size=0.09;
   //-------------------------------------------------------------------//



  TCanvas *c1 = new TCanvas("c1"," ",200,10,1200,800);
  //c1->cd(1)->SetFillColor(38);
  
  TGraph *csda_range_vs_mom=new TGraph(mom.size(), &mom[0], &csda_range[0]);
  TGraph *csda_range_vs_mom_sm=new TGraph(mom.size(), &mom[0], &csda_range[0]);
  
  csda_range_vs_mom->GetXaxis()->SetTitle("Proton Momentum [GeV/c]");
  csda_range_vs_mom->GetYaxis()->SetTitle("CSDA Range [cm]");
  csda_range_vs_mom->SetMarkerSize(2.4);
  csda_range_vs_mom->SetMarkerStyle(20);
  csda_range_vs_mom->SetMarkerColor(2);
  csda_range_vs_mom->Draw("ap");


  TGraphSmooth *gs = new TGraphSmooth("supsmu");
  //csda_range_vs_mom_sm= gs->Approx(csda_range_vs_mom_sm,"linear",40);
  csda_range_vs_mom_sm= gs->Approx(csda_range_vs_mom_sm,"linear",4000);
  csda_range_vs_mom_sm->SetLineColor(1);
  csda_range_vs_mom_sm->Draw("same");

  //get numerical values from TGraphSmooth function
  double mom_val=1.;
  double csda_val=csda_range_vs_mom_sm->Eval(mom_val);
  std::cout<<"mom="<<mom_val<<" csda_val="<<csda_val<<std::endl;

  TLegend *leg=new TLegend(0.203673,0.645914,0.573456,0.83917);
  leg->SetLineColor(0);
  TLegendEntry* ll[2];
  ll[0]=leg->AddEntry((TObject*)csda_range_vs_mom,Form("%s","NIST Data Base"),"p");
  ll[1]=leg->AddEntry((TObject*)csda_range_vs_mom_sm,Form("%s","Friedman's Super Smoother"),"l");
  ll[0]->SetTextColor(2);
  ll[1]->SetTextColor(1);
  //leg->SetNColumns(2);
  leg->Draw();

  //swap x-and y-axis
  TCanvas *c2 = new TCanvas("c2"," ",200,10,1000,800);
  
  TGraph *mom_vs_csda_range=new TGraph(mom.size(), &csda_range[0], &mom[0]);
  TGraph *mom_vs_csda_range_sm=new TGraph(mom.size(), &csda_range[0], &mom[0]);
  
  mom_vs_csda_range->GetYaxis()->SetTitle("Proton Momentum [GeV/c]");
  mom_vs_csda_range->GetXaxis()->SetTitle("CSDA Range [cm]");
  mom_vs_csda_range->SetMarkerSize(2.4);
  mom_vs_csda_range->SetMarkerStyle(20);
  mom_vs_csda_range->SetMarkerColor(2);
  mom_vs_csda_range->Draw("ap");


  TGraphSmooth *gss = new TGraphSmooth("supsmu");
  //csda_range_vs_mom_sm= gs->Approx(csda_range_vs_mom_sm,"linear",40);
  mom_vs_csda_range_sm= gss->Approx(mom_vs_csda_range_sm,"linear",40000000);
  mom_vs_csda_range_sm->SetLineColor(1);
  mom_vs_csda_range_sm->Draw("same");


  //get numerical values from TGraphSmooth function
  double csda_val2=92.5839;
  double mom_val2=mom_vs_csda_range_sm->Eval(csda_val2);
  std::cout<<"csda="<<csda_val2<<" mom_val="<<mom_val2<<std::endl;



  //Save results
  csda_range_vs_mom->SetName("csda_range_vs_mom");
  csda_range_vs_mom_sm->SetName("csda_range_vs_mom_sm");
  mom_vs_csda_range->SetName("mom_vs_csda_range");
  mom_vs_csda_range_sm->SetName("mom_vs_csda_range_sm");
  TFile *fout=new TFile("proton_mom_csda_converter.root","create");
    csda_range_vs_mom->Write();
    csda_range_vs_mom_sm->Write();
    mom_vs_csda_range->Write();
    mom_vs_csda_range_sm->Write();
 fout->Close();





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

  //TSpline3 *s3 = new TSpline3(“s3”,csda_range_vs_mom->GetX(),csda_range_vs_mom->GetY(),csda_range_vs_mom->GetN());
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
