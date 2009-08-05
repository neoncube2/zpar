
   /*----------------------word bigram----------------------*/
   head_word_tag_dep_word_tag.allocate( head_word_tag, dep_word_tag ) ;
   head_word_tag_dep_word.allocate( head_word_tag, dep_word_nil ) ;
   head_word_dep_word_tag.allocate( head_word_nil, dep_word_tag ) ;
   head_word_tag_dep_tag.allocate( head_word_tag, dep_nil_tag ) ;
   head_tag_dep_word_tag.allocate( head_nil_tag, dep_word_tag ) ;
   head_word_dep_word.allocate( head_word, dep_word ) ;

#define updateUnigrams(x) \
   cast_weights->m_mapHeadWordTag[ make_pair(head_word_tag, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepWordTag[ make_pair(dep_word_tag, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWord[ make_pair(head_word, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepWord[ make_pair(dep_word, x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTag[ make_pair(head_tag.code(), x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapDepTag[ make_pair(dep_tag.code(), x) ].updateCurrent( amount , round ) ;

#define updateBigrams(x) \
   cast_weights->m_mapHeadWordTagDepWordTag[ make_pair(head_word_tag_dep_word_tag,x) ].updateCurrent( amount , round ); \
   cast_weights->m_mapHeadWordTagDepWord[ make_pair(head_word_tag_dep_word,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordDepWordTag[ make_pair(head_word_dep_word_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordTagDepTag[ make_pair(head_word_tag_dep_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTagDepWordTag[ make_pair(head_tag_dep_word_tag,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadWordDepWord[ make_pair(head_word_dep_word,x) ].updateCurrent( amount , round ) ; \
   cast_weights->m_mapHeadTagDepTag[ make_pair((head_tag.code()<<PENN_TAG_COUNT_BITS)+dep_tag.code(),x) ].updateCurrent( amount , round ) ;

#define updateSurrounding(x) \
   both_sides = (head_tag.code()<<PENN_TAG_COUNT_BITS*2) + 0 + dep_tag.code() ; \
   for ( index=left_index+1; index<right_index; index++ ) { \
      between_tags = both_sides + (m_lCache[index].tag.code()<<PENN_TAG_COUNT_BITS) ; \
      cast_weights->m_mapBetweenTags[ make_pair(between_tags, x) ].updateCurrent( amount , round ) ; \
   } \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ make_pair(encodeTags(head_tag, head_tag_l, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ make_pair(encodeTags(head_tag, head_tag_r, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ make_pair(encodeTags(PENN_TAG_NONE, head_tag_l, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ make_pair(encodeTags(PENN_TAG_NONE, head_tag_l, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ make_pair(encodeTags(PENN_TAG_NONE, head_tag_r, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ make_pair(encodeTags(PENN_TAG_NONE, head_tag_r, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_ll) cast_weights->m_mapSurroundingTagsLL[ make_pair(encodeTags(head_tag, head_tag_l, PENN_TAG_NONE, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_lr) cast_weights->m_mapSurroundingTagsLR[ make_pair(encodeTags(head_tag, head_tag_l, PENN_TAG_NONE, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_rl) cast_weights->m_mapSurroundingTagsRL[ make_pair(encodeTags(head_tag, head_tag_r, PENN_TAG_NONE, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc_rr) cast_weights->m_mapSurroundingTagsRR[ make_pair(encodeTags(head_tag, head_tag_r, PENN_TAG_NONE, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc__l) cast_weights->m_mapSurroundingTagsLL[ make_pair(encodeTags(head_tag, PENN_TAG_NONE, dep_tag, dep_tag_l), x) ].updateCurrent( amount , round ) ; \
   if (nc__r) cast_weights->m_mapSurroundingTagsLR[ make_pair(encodeTags(head_tag, PENN_TAG_NONE, dep_tag, dep_tag_r), x) ].updateCurrent( amount , round ) ; \
   if (nc_l_) cast_weights->m_mapSurroundingTagsLR[ make_pair(encodeTags(head_tag, head_tag_l, dep_tag, PENN_TAG_NONE), x) ].updateCurrent( amount , round ) ; \
   if (nc_r_) cast_weights->m_mapSurroundingTagsRR[ make_pair(encodeTags(head_tag, head_tag_r, dep_tag, PENN_TAG_NONE), x) ].updateCurrent( amount , round ) ;

