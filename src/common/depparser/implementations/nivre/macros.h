#ifndef _GENERAL_DEPPARSER_MACROS_H
#define _GENERAL_DEPPARSER_MACROS_H

// early update? 
#define EARLY_UPDATE 1

// supertag
#define SR_SUPERTAG 1
typedef double SCORE_TYPE ;

// The size of agenda
const int AGENDA_SIZE = 64 ;

// The size of a sentence and the words
const int MAX_SENTENCE_SIZE = 256 ; 
const int MAX_SENTENCE_SIZE_BITS = 8 ; 

// link size and direction are combined
const int LINK_DIRECTION_HEAD_LEFT = -7 ; // head on the left
const int LINK_DIRECTION_HEAD_RIGHT = 7 ; // on the right

// normalise link size and the direction
inline int getLinkSizeAndDirection(const int &head_index, const int &dep_index) {
   static int diff;
   diff = head_index - dep_index;
   assert(diff != 0); 
   if (diff>10) diff = 6; 
   else if (diff>5) diff = 5; 
   if (diff<-10) diff = -6; 
   else if (diff<-5) diff = -5;
   return diff;
}
inline int getLinkDirection(const int &head_index, const int &dep_index) {
   return head_index>dep_index ? 0 : 1 ;
}
inline int getLinkDirectionEncode(const int &head_index, const int &dep_index) {
   return head_index>dep_index ? LINK_DIRECTION_HEAD_RIGHT : LINK_DIRECTION_HEAD_LEFT ;
}
#ifdef LABELED
inline int getLabelAndDirectionOrDistanceEncode(const unsigned long &label, const int &dod) {
   return (dod<<PENN_DEP_COUNT)+label;
}
#endif

// arity direction
enum ARITY_DIRECTION { ARITY_DIRECTION_LEFT=0, ARITY_DIRECTION_RIGHT=1 } ;

#endif
