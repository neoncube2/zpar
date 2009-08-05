/****************************************************************
 *                                                              *
 * reranker.cpp - the reranker for segmented and tagged outputs *
 *                                                              *
 * It updates the segmentor and tagger in a unified manner      *
 *                                                              *
 * Yue Zhang                                                    *
 * 2008.03                                                      *
 *                                                              *
 ****************************************************************/

#include "reranker.h"
#include "writer.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 * 
 *---------------------------------------------------------------*/

void recordSegmentation(const CSentenceRaw* raw, const CSentenceRaw* segmented, CBitArray &retval) {
   vector<int> indice;
   for (int i=0; i<raw->size(); ++i) {
      for (int j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   retval.clear();
   int start=0;
   for (int i=0; i<segmented->size(); ++i) {
      int word_start = indice[start];
      int word_end = indice[start+segmented->at(i).size()-1];
      start += segmented->at(i).size();
      retval.set(word_end);
   }
}

/*----------------------------------------------------------------
 *
 * findBest - find the best segmented and tagged sequence given
 *            raw input. Stored at scratch_seg and scratch_tag [best_index] respectively
 * 
 *---------------------------------------------------------------*/

int CReranker::findBest(const CSentenceRaw &raw) {
   int best_index=0;
   double best_score;
   static CBitArray wds(MAX_SENTENCE_SIZE);
   m_segmentor->segment(&raw, m_scratch_seg, m_scores_seg, m_nBest);
   for (int i=0; i<m_nBest; i++) {
      recordSegmentation(&raw, m_scratch_seg+i, wds);
      m_tagger->tag(&raw, m_scratch_tag+i, m_scores_tag+i, 1, &wds); 
      if (m_scratch_tag[i].empty()) continue;
      if (i==0 || m_scores_tag[i]+m_scores_seg[i]>best_score) {best_score=m_scores_tag[i]+m_scores_seg[i]; best_index=i;}
   }
   return best_index;
}

/*----------------------------------------------------------------
 *
 * decode - decoding interface
 * 
 *---------------------------------------------------------------*/

void CReranker::decode(const CSentenceRaw &raw, CSentenceTagged &tagged) {
   assert(!m_bTrain);
   int best_index = findBest(raw);
   tagged = m_scratch_tag[best_index];
}

/*----------------------------------------------------------------
 *
 * train - training interface
 * 
 *---------------------------------------------------------------*/

void CReranker::train(const CSentenceTagged &correct) {
   assert(m_bTrain);
   ++m_nTrainingRound;
   CSentenceRaw raw; CSentenceRaw segmented;
   UntagAndDesegmentSentence(&correct, &raw); UntagSentence(&correct, &segmented);
   CSentenceWriter writer("");
   int best_index = findBest(raw);
   assert(m_scratch_seg[best_index].empty()==false);
   assert(m_scratch_tag[best_index].empty()==false);
   if ( m_scratch_seg[best_index] != segmented ) {
      cout << "Segmentation error" << endl;
      m_segmentor->updateScores(m_scratch_seg+best_index, &segmented, m_nTrainingRound);
      ++m_nTrainingErrors;
   }
   if ( m_scratch_tag[best_index] != correct ) {
      writer.writeSentence(&correct);
      writer.writeSentence(m_scratch_tag+best_index);
      ++m_nTrainingErrort;
   }
   // have to update the tagger anyway because it maintains useful info
   m_tagger->updateScores(m_scratch_tag+best_index, &correct, m_nTrainingRound);
}

/*----------------------------------------------------------------
 *
 * train - training interface
 * 
 *---------------------------------------------------------------*/

void CReranker::train_separate(const CSentenceTagged &correct) {
   assert(m_bTrain);
   ++m_nTrainingRound;
   CSentenceRaw raw; CSentenceRaw segmented;
   static CBitArray wds(MAX_SENTENCE_SIZE);
   UntagAndDesegmentSentence(&correct, &raw); 
   UntagSentence(&correct, &segmented);
   CSentenceWriter writer("");

   m_segmentor->segment(&raw, m_scratch_seg);
   if ( m_scratch_seg[0] != segmented ) {
      cout << "Segmentation error" << endl;
      m_segmentor->updateScores(m_scratch_seg, &segmented, m_nTrainingRound);
      ++m_nTrainingErrors;
   }

   recordSegmentation(&raw, &segmented, wds);
   m_tagger->tag(&raw, m_scratch_tag, NULL, 1, &wds); 
   if ( m_scratch_tag[0] != correct ) {
      cout << "Tagging error" << endl;
      ++m_nTrainingErrort;
   }
   // have to update the tagger anyway because it maintains useful info
   m_tagger->updateScores(m_scratch_tag, &correct, m_nTrainingRound);
}

/*----------------------------------------------------------------
 *
 * finishTraining - must_call
 * 
 *---------------------------------------------------------------*/

void CReranker::finishTraining() {
   m_segmentor->finishTraining(m_nTrainingRound);
   m_tagger->finishTraining(m_nTrainingRound);
   cout << m_nTrainingRound << " training sentences in total have been processed; " << m_nTrainingErrors << " segmentation mistakes and  " << m_nTrainingErrort << " tagging mistakes." << endl;
}

