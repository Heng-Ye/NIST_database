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

//argon density -------------------------------//
double rho_Ar=1.3954; //liquid argon density [g/cm3]
//---------------------------------------------//

void fit_dedx_ke(){

  vector<double> data;
  vector<double> ke;
  vector<double> dedx;
  vector<double> ex;
  string line;

  int n=2; //2 columns
  double buffer;
  //ifstream f_in("proton_dedx_ke_mev.dat");
  ifstream f_in("proton_ke_stoppingpower.dat");
  while (f_in>>buffer) { data.push_back(buffer); }
  f_in.close();
  
  for (int k=0; k<data.size(); k+=n) {
    ke.push_back(data[k]); //KE in MeV
    dedx.push_back(rho_Ar*(double)data[k+1]); //input::stopping power [MeV*cm2/g] --> make it to MeV/cm
    ex.push_back(0);	
  } 

  for (int k=0; k<ke.size(); k++) {
    std::cout<<ke[k]<<" "<<dedx[k]<<std::endl;
  }


   //settting for figures ----------------------------------------------//
/*
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

   //gStyle->SetTitleOffset(1.,"xy");
   gStyle->SetTitleOffset(.9,"y");
   gStyle->SetTitleOffset(1.1,"x");

   //gStyle->SetTitleOffset(1.2,"y");
   gStyle->SetTitleOffset(1.2,"t");
   //float global_btmargin=0.15;

   gStyle->SetPadBottomMargin(0.14);
   gStyle->SetPadLeftMargin(0.13);
   //gStyle->SetPadRightMargin(0.1);
   gStyle->SetPaintTextFormat("5.2f");
   Float_t lt_size=0.09;
*/

        gROOT->LoadMacro(" ~/protoDUNEStyle.C"); //load pDUNE style
        gROOT->SetStyle("protoDUNEStyle");
        gROOT->ForceStyle();
        gStyle->SetTitleX(0.5);
        gStyle->SetTitleAlign(23);
   	gStyle->SetTitleOffset(1.2,"x");
        gStyle->SetOptStat(0);




   //-------------------------------------------------------------------//



  TCanvas *c1 = new TCanvas("c1"," ",200,10,900,600);
  //c1->cd(1)->SetFillColor(38);
  c1->cd(1)->SetLogy();
  c1->cd(1)->SetLogx();
  
  TGraph *dedx_vs_ke=new TGraph(ke.size(), &ke[0], &dedx[0]);
  TGraph *dedx_vs_ke_sm=new TGraph(ke.size(), &ke[0], &dedx[0]);
  
  dedx_vs_ke->SetTitle("Protons in LAr");
  dedx_vs_ke->GetXaxis()->SetTitle("Proton Kinetic Energy [MeV]");
  dedx_vs_ke->GetYaxis()->SetTitle("dE/dx [MeV/cm]");
  dedx_vs_ke->SetMarkerSize(2.4);
  dedx_vs_ke->SetMarkerStyle(20);
  dedx_vs_ke->SetMarkerColor(2);

  TH2D *f2d=new TH2D("f2d","",100,0.001,10000,10,0.5,5000); 
  f2d->SetTitle("Protons in LAr; Proton Kinetic Energy [MeV]; dE/dx [MeV/cm]");
  f2d->Draw();
  //dedx_vs_ke->Draw("ap same");
  dedx_vs_ke->Draw("p same");


  TGraphSmooth *gs = new TGraphSmooth("supsmu");
  //dedx_vs_ke_sm= gs->Approx(dedx_vs_ke_sm,"linear",40);
  //dedx_vs_ke_sm= gs->Approx(dedx_vs_ke_sm,"linear",10000000);
  dedx_vs_ke_sm= gs->Approx(dedx_vs_ke_sm,"linear",1000000);
  dedx_vs_ke_sm->SetLineColor(1);
  dedx_vs_ke_sm->Draw("same");

  //get numerical values from TGraphSmooth function
  //std::cout<<"1 GeV/c proton has KE:"<<p2ke(1.)<<" GeV"<<std::endl;

  double ke_val=p2ke(1.);
  double csda_val=dedx_vs_ke_sm->Eval(ke_val);
  std::cout<<"ke="<<ke_val<<" csda_val="<<csda_val<<std::endl;

  //TLegend *leg=new TLegend(0.203673,0.645914,0.573456,0.83917);
  TLegend *leg=new TLegend(0.511044,0.71968,0.874498,0.875286);
  leg->SetLineColor(0);
  TLegendEntry* ll[2];
  ll[0]=leg->AddEntry((TObject*)dedx_vs_ke,Form("%s","NIST Data Base"),"p");
  ll[1]=leg->AddEntry((TObject*)dedx_vs_ke_sm,Form("%s","Friedman's Super Smoother"),"l");
  ll[0]->SetTextColor(2);
  ll[1]->SetTextColor(1);
  //leg->SetNColumns(2);
  leg->Draw();


  c1->Print("Protons_inAr_dEdx_vs_KE.eps");


  //Save results
  dedx_vs_ke->SetName("dEdx_vs_KE");
  dedx_vs_ke_sm->SetName("dEdx_vs_KE_sm");
  TFile *fout=new TFile("proton_ydedx_xke.root","create");
    dedx_vs_ke->Write();
    dedx_vs_ke_sm->Write();
 fout->Close();



/*
  TCanvas *c2 = new TCanvas("c2"," ",200,10,1000,900);
  TGraph *ke_vs_dedx=new TGraph(ke.size(), &dedx[0], &ke[0]);
  TGraph *ke_vs_dedx_sm=new TGraph(ke.size(), &dedx[0], &ke[0]);
  
  ke_vs_dedx->GetYaxis()->SetTitle("Proton Kinetic Energy [MeV]");
  ke_vs_dedx->GetXaxis()->SetTitle("dE/dx [MeV/cm]");
  ke_vs_dedx->SetMarkerSize(2.4);
  ke_vs_dedx->SetMarkerStyle(20);
  ke_vs_dedx->SetMarkerColor(2);
  ke_vs_dedx->Draw("ap");

  TGraphSmooth *gs2 = new TGraphSmooth("supsmu");
  ke_vs_dedx_sm= gs2->Approx(ke_vs_dedx_sm,"linear",10000000);
  ke_vs_dedx_sm->SetLineColor(1);
  ke_vs_dedx_sm->Draw("same");
*/







/*

  //Save results
  dedx_vs_ke->SetName("dedx_vs_ke");
  dedx_vs_ke_sm->SetName("dedx_vs_ke_sm");
  TFile *fout=new TFile("proton_ydedx_xke.root","create");
    dedx_vs_ke->Write();
    dedx_vs_ke_sm->Write();
 fout->Close();

*/



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
