#include "GSSWGraph.h"
#include <mutex>
#include <vector>

uint32_t GSSWGraph::s_next_id = 0;
std::mutex GSSWGraph::s_lock;
GSSWGraph::GSSWGraph(IReference::SharedPtr referencePtr, IVariantList::SharedPtr variantListPtr, Region::SharedPtr regionPtr, int matchValue, int misMatchValue, int gapOpenValue, int gapExtensionValue, uint32_t numGraphCopies) :
  IGraph(referencePtr, variantListPtr),
  m_match(matchValue),
  m_mismatch(misMatchValue),
  m_gap_open(gapOpenValue),
  m_gap_extension(gapExtensionValue),
  m_variant_list_ptr(variantListPtr),
  m_region_ptr(regionPtr),
  m_total_graph_length(0),
  m_skipped(false),
  m_num_graph_copies(numGraphCopies)
{
  this->m_nt_table = gssw_create_nt_table();
  this->m_mat = gssw_create_score_matrix(this->m_match, this->m_mismatch);
  this->m_graph_ptr = gssw_graph_create(100);
}

GSSWGraph::~GSSWGraph()
{
}

void GSSWGraph::constructGraph() {
  int64_t referenceSize;
  IVariant::SharedPtr variantPtr = nullptr;
  std::vector< gssw_node* > altAndRefVertices;
  position currentReferencePosition = this->m_region_ptr->getStartPosition();

  while(this->m_variant_list_ptr->getNextVariant(variantPtr)){
    if (variantPtr->shouldSkip()){
      m_skipped = true;
      continue;
    }
    referenceSize = variantPtr->getPosition() - currentReferencePosition;
    if (referenceSize > 0) {
      auto refRegionPtr = std::make_shared< Region >(this->m_region_ptr->getReferenceID(), currentReferencePosition, variantPtr->getPosition() - 1, Region::BASED::ONE);
      std::string referenceSequenceString = this->m_reference_ptr->getSequenceFromRegion(refRegionPtr);
      auto referenceAllelePtr = std::make_shared< Allele >(referenceSequenceString);
      auto referenceNode = addReferenceVertex(variantPtr->getRegions()[0]->getStartPosition(), referenceAllelePtr, altAndRefVertices);
      altAndRefVertices.clear();
      altAndRefVertices.push_back(referenceNode);
      m_total_graph_length += referenceSize;
      currentReferencePosition += referenceSize;
    }
    altAndRefVertices = addAlternateVertices(altAndRefVertices, variantPtr);
    currentReferencePosition += variantPtr->getReferenceSize();
  }
  referenceSize = this->m_region_ptr->getEndPosition() - currentReferencePosition;
  if(referenceSize > 0){
    auto refRegionPtr = std::make_shared< Region >(this->m_region_ptr->getReferenceID(), currentReferencePosition, this->m_region_ptr->getEndPosition(), Region::BASED::ONE);
    std::string referenceSequenceString = this->m_reference_ptr->getSequenceFromRegion(refRegionPtr);
    auto referenceAllelePtr = std::make_shared< Allele >(referenceSequenceString);
    addReferenceVertex(currentReferencePosition, referenceAllelePtr, altAndRefVertices);
  }
  generateGraphCopies();
  
}

gssw_node* GSSWGraph::addReferenceVertex(position position, IAllele::SharedPtr referenceAllelePtr, std::vector< gssw_node* > altAndRefVertices) {
  this->m_reference_fragments.emplace_back(referenceAllelePtr);
  auto referenceNodePtr = gssw_node_create_alt(position, referenceAllelePtr->getSequence(), referenceAllelePtr->getLength(), referenceAllelePtr, true, this->m_nt_table, this->m_mat);
  gssw_graph_add_node(this->m_graph_ptr, referenceNodePtr);
  for (auto iter = altAndRefVertices.begin(); iter != altAndRefVertices.end(); ++iter){
    gssw_nodes_add_edge((*iter), referenceNodePtr);
  }


  return referenceNodePtr;
}

std::vector< gssw_node* > GSSWGraph::addAlternateVertices(const std::vector< gssw_node*>& altAndRefVertices, IVariant::SharedPtr variantPtr){
  size_t tmpLength = 0;
  std::vector< gssw_node* > vertices;
  for(auto altAllelePtr : variantPtr->getAltAllelePtrs()){
    auto altAlleleNode = gssw_node_create_alt(variantPtr->getPosition(), variantPtr->getRefAllelePtr()->getSequence(), variantPtr->getRefAllelePtr()->getLength(), altAllelePtr, false, this->m_nt_table, this->m_mat);
    gssw_graph_add_node(this->m_graph_ptr, altAlleleNode);
    vertices.emplace_back(altAlleleNode);
    if(altAllelePtr->getLength() > tmpLength) {tmpLength = altAllelePtr->getLength(); }
    
  }
  gssw_node* variantReferenceNode = gssw_node_create_alt(variantPtr->getPosition(), variantPtr->getRefAllelePtr()->getSequence(), variantPtr->getRefAllelePtr()->getLength(), variantPtr->getRefAllelePtr(), true, this->m_nt_table, this->m_mat);

  if(variantPtr->getRefAllelePtr()->getLength() > tmpLength) {tmpLength = variantPtr->getRefAllelePtr()->getLength();}
  
  gssw_graph_add_node(this->m_graph_ptr, variantReferenceNode);
  vertices.push_back(variantReferenceNode);
  for(auto parentNode : altAndRefVertices){
    for(auto childNode : vertices){
      gssw_nodes_add_edge(parentNode, childNode);
    }
  }
  return vertices;
}
