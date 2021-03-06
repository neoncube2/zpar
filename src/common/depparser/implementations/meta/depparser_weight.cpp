// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency parser of english                     *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 * Affects: m_bScoreModified, clearing it.
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::loadScores() {
   clock_t time_start = clock();
   std::cerr<<"Loading scores..."; std::cerr.flush();
   std::ifstream file ;
   std::string s;
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cerr << " empty." << std::endl; return;
   }

#ifdef LABELED
//   getline(file, s);
//   ASSERT(s=="Dependency labels:", "Dependency labels not found in model file") ;
//   getline(file, s);
//   std::istringstream iss(s);
//   CDependencyLabel label;
//   while(iss >> label);
//   getline(file, s);
//   ASSERT(s=="", "No empty line after the dependency labels") ;
#endif
   iterate_templates(file >>,,;);

//   getline(file, s);
//   if (s=="Rules=1") {
//      setRules(true);
//   }
//   else {
//      ASSERT(s=="Rules=0", "Rules flag not found from the model file");
//      setRules(false);
//   }

   file.close() ;
   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cerr<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::saveScores() {
   std::cerr<<"Saving scores..."; std::cerr.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

#ifdef LABELED
//   file << "Dependency labels:" << std::endl;
//   for (unsigned label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label)
//      file << CDependencyLabel(label) << ' ';
//   file << std::endl << std::endl;
#endif
   iterate_templates(file<<,,;)
#ifdef DEBUG
   iterate_templates(,,.trace(););
#endif
//   if (m_bRules) file << "Rules=1" << std::endl;
//   else file << "Rules=0" << std::endl;

   file.close();
   std::cerr<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::computeAverageFeatureWeights(int round) {
   std::cerr<<"Computing averaged (total) feature vector..."; std::cerr.flush();
   iterate_templates(,,.computeAverage(round);) ;

   std::cerr<<"done."<<std::endl;
}

//================================================================

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 * Affects: m_bScoreModified, clearing it.
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeightPlusMeta::loadScores() {
   clock_t time_start = clock();
   std::cerr<<"Loading scores..."; std::cerr.flush();
   std::ifstream file ;
   std::string s;
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cerr << " empty." << std::endl; return;
   }

#ifdef LABELED
   getline(file, s);
   ASSERT(s=="Dependency labels:", "Dependency labels not found in model file") ;
   getline(file, s);
   std::istringstream iss(s);
   CDependencyLabel label;
   while(iss >> label);
   getline(file, s);
   ASSERT(s=="", "No empty line after the dependency labels") ;
#endif
   iterate_templates(file >>,,;);
   iterate_templates(file >>,m_meta.,;);
   iterate_templates(file >>,m_metapos.,;);

   getline(file, s);
   if (s=="Rules=1") {
      setRules(true);
   }
   else {
      ASSERT(s=="Rules=0", "Rules flag not found from the model file");
      setRules(false);
   }

   file.close() ;
   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cerr<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeightPlusMeta::saveScores() {
   std::cerr<<"Saving scores..."; std::cerr.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

#ifdef LABELED
   file << "Dependency labels:" << std::endl;
   for (unsigned label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label)
      file << CDependencyLabel(label) << ' ';
   file << std::endl << std::endl;
#endif
   iterate_templates(file<<,,;)
   iterate_templates(file<<,m_meta.,;)
   iterate_templates(file<<,m_metapos.,;)
#ifdef DEBUG
   iterate_templates(,,.trace(););
   iterate_templates(,m_meta.,.trace(););
   iterate_templates(,m_metapos.,.trace(););
#endif
   if (m_bRules) file << "Rules=1" << std::endl;
   else file << "Rules=0" << std::endl;

   file.close();
   std::cerr<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeightPlusMeta::computeAverageFeatureWeights(int round) {
   std::cerr<<"Computing averaged (total) feature vector..."; std::cerr.flush();
   iterate_templates(,,.computeAverage(round);) ;
   iterate_templates(,m_meta.,.computeAverage(round);) ;
   iterate_templates(,m_metapos.,.computeAverage(round);) ;

   std::cerr<<"done."<<std::endl;
}

