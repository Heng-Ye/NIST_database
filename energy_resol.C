
double m_proton=0.938272046;


double p2ke(double p) {
 double ke=m_proton*(-1.+sqrt(1.+(p*p)/(m_proton*m_proton)));
 return ke;
}


double ke2p(double ke) {
 double p=sqrt(ke*(ke+2*m_proton));
 return p;
}

double dke2dp(double dke) {
 //double dp=sqrt(ke*(ke+2*m_proton));
 return dp;
}


void energy_resol() {

 double dke=0.102;
 double ke=0.398;

 double dp=dke2dp(dke);
 double p=ke2p(ke);
 double presol=100.*dp/p;

 cout<<"p:"<<p<<endl;
 cout<<"dp:"<<dp<<endl;
 cout<<"presol:"<<presol<<endl;



//15.30% for dp/p

}
