#ifndef ModeMesure_hpp
#define ModeMesure_hpp

#include <cinttypes>
#include <array>

class ModeMesure {
private:
  // TIme start ,finished et total serve pour calculer le temps de vols
  uint64_t timestart, timefinished,  timetotal ;
  // TimePauseStart and end sont les temps pendant que on est entrain de toucher le trampolin
  uint64_t timePauseStart , timePauseEnd ;
  // Le temps de vol
  uint64_t time_us , totalTempsPause;
  // Bool pause , mode 1 et mode 2 sont les boolean qui servent a savoir si on est respectivement dans la pause , mode 1 ou mode 2
  bool boolPause , boolMode1 , boolMode2; 

  bool boolPresencePersonne = false ;
  bool boolBouton = false ; 
  bool boolLancerMesure = false ; 
  bool boolEntrainDeMesurer = false ;


  //std::array<uint64_t, 10> tabTemps ;
  std::array<uint32_t, 10> tabTemps ;
  int indiceTabTemps;


public:
  uint64_t lectureTemps();
  void init();
  //std::array<uint64_t, 10> getTabTemps() ;
  std::array<uint32_t, 10> getTabTemps() ;

  void absencePersonne();
  bool presencePersonne();

  void presenceBouton();

  void lancerMesure();

  int getIndice();

  void cleanTab();



  uint64_t getTime();
};

#endif /* end of include guard:ModeMesure_hpp */