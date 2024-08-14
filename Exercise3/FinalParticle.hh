#ifndef MarcelloFinalParticle_h
#define MarcelloFinalParticle_h

#include "globals.hh"
#include "G4ThreeVector.hh"

namespace Marcello{

  class FinalParticle{
  public:
    FinalParticle(){}
    ~FinalParticle(){}
    void clear(){
      pfid=0;
      pfpid=0;
      pfpname="";
      pfpartid=0;
      pfiProc="";
      pfiKEn=0.;
      pfiPos=G4ThreeVector(0.,0.,0.);
      pfiVol="";
      pfiTim=0.;
      pffProc="";
      pffKEn=0.;
      pffPos=G4ThreeVector(0.,0.,0.);
      pffVol="";
      pffTim=0.;      
    }
    
    void setTrackId(G4int id){pfid=id;}
    void setParentTrackId(G4int pid){pfpid=pid;}
    void setParticleName(const G4String& pname){pfpname=pname;}
    void setParticleId(G4int partid){pfpartid=partid;}
    
    void setInitProcName(const G4String& iProc){pfiProc=iProc;}
    void setInitKEne(G4double iKEn){pfiKEn=iKEn;}
    void setInitPos(const G4ThreeVector & iPos){pfiPos=iPos;}
    void setInitVol(const G4String& iVol){pfiVol=iVol;}
    void setInitTime(G4double iTim){pfiTim=iTim;}

    void setFinProcName(const G4String& fProc){pffProc=fProc;}
    void setFinKEne(G4double fKEn){pffKEn=fKEn;}
    void setFinPos(const G4ThreeVector & fPos){pffPos=fPos;}
    void setFinVol(const G4String& fVol){pffVol=fVol;}
    void setFinTime(G4double fTim){pffTim=fTim;}

    G4int getTrackId() const { return pfid; }
    G4int getParentTrackId() const { return pfpid; }
    G4String getParticleName() const { return pfpname; }
    G4int getParticleId() const { return pfpartid; }

    G4String getInitProcName() const { return pfiProc; }
    G4double getInitKEne() const { return pfiKEn; }
    G4ThreeVector getInitPos() const { return pfiPos; }
    G4String getInitVol() const { return pfiVol; }
    G4double getInitTime() const { return pfiTim; }

    G4String getFinProcName() const { return pffProc; }
    G4double getFinKEne() const { return pffKEn; }
    G4ThreeVector getFinPos() const { return pffPos; }
    G4String getFinVol() const { return pffVol; }
    G4double getFinTime() const { return pffTim; }


    G4int TrackId(){return pfid;}
    G4int ParentTrackId(){return pfpid;}
    const G4String& ParticleName(){return pfpname;}
    G4int ParticleId(){return pfpartid;}
    
    const G4String& InitProcName(){return pfiProc;}
    G4double InitKEne(){return pfiKEn;}
    const G4ThreeVector& InitPos(){return pfiPos;}
    const G4String& InitVol(){return pfiVol;}
    G4int InitTime(){return pfiTim;}

    const G4String& FinProcName(){return pffProc;}
    G4double FinKEne(){return pffKEn;}
    const G4ThreeVector& FinPos(){return pffPos;}
    const G4String& FinVol(){return pffVol;}
    G4int FinTime(){return pffTim;}
    
    friend std::ostream& operator<<(std::ostream& outp, Marcello::FinalParticle p){
      outp<<" Trid "<<p.TrackId()<<" parentTrId "<<p.ParentTrackId()<<" particle "<<p.ParticleName()<<" id "<<p.ParticleId()
	  <<" INITIAL process "<<p.InitProcName()<<" KinEne "<<p.InitKEne()<<" Pos "<<p.InitPos()<<" Volume "<<p.InitVol()<<" time "<<p.InitTime()
	  <<" FINAL process "<<p.FinProcName()<<" KinEne "<<p.FinKEne()<<" Pos "<<p.FinPos()<<" Volume "<<p.FinVol()<<" time "<<p.FinTime();
      return outp;
    }
    

  private:
    G4int         pfid;
    G4int         pfpid;
    G4String      pfpname;
    G4int         pfpartid;
    G4String      pfiProc;
    G4double      pfiKEn;
    G4ThreeVector pfiPos;
    G4String      pfiVol;
    G4double      pfiTim;
    G4String      pffProc;
    G4double      pffKEn;
    G4ThreeVector pffPos;
    G4String      pffVol;
    G4double      pffTim;		  
  };
}
#endif
