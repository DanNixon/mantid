#include "MantidAPI/ComponentInfo.h"
#include "MantidGeometry/IComponent.h"
#include "MantidBeamline/ComponentInfo.h"
#include <boost/make_shared.hpp>
#include <exception>
#include <string>
#include <boost/make_shared.hpp>
#include <sstream>

namespace Mantid {
namespace API {

/**
 * Constructor.
 * @param componentInfo : Internal Beamline ComponentInfo
 * @param componentIds : ComponentIDs ordered by component
 * @param componentIdToIndexMap : ID -> index translation map
 * index
 */
ComponentInfo::ComponentInfo(
    const Mantid::Beamline::ComponentInfo &componentInfo,
    boost::shared_ptr<const std::vector<Mantid::Geometry::IComponent *>>
        componentIds,
    boost::shared_ptr<const std::unordered_map<Geometry::IComponent *, size_t>>
        compIdToIndexMap)
    : m_componentInfo(componentInfo), m_componentIds(std::move(componentIds)),
      m_compIDToIndex(std::move(compIdToIndexMap)) {

  if (m_componentIds->size() != m_compIDToIndex->size()) {
    throw std::invalid_argument(
        "Inconsistent ID and Mapping input containers for API::ComponentInfo");
  }
  if (m_componentIds->size() != m_componentInfo.size()) {
    throw std::invalid_argument("Inconsistent ID and base "
                                "Beamline::ComponentInfo sizes for "
                                "API::ComponentInfo");
  }
}

std::vector<size_t>
ComponentInfo::detectorIndices(size_t componentIndex) const {
  return m_componentInfo.detectorIndices(componentIndex);
}

boost::shared_ptr<const std::vector<Geometry::IComponent *>>
ComponentInfo::componentIds() const {
  return m_componentIds;
}

size_t ComponentInfo::size() const { return m_componentInfo.size(); }

size_t ComponentInfo::indexOf(Geometry::IComponent *id) const {
  return m_compIDToIndex->at(id);
}

bool ComponentInfo::operator==(const ComponentInfo &other) const {
  return this->m_componentInfo == other.m_componentInfo &&
         m_compIDToIndex == other.m_compIDToIndex;
}

bool ComponentInfo::operator!=(const ComponentInfo &other) const {
  return !this->operator==(other);
}

boost::shared_ptr<
    const std::unordered_map<Mantid::Geometry::IComponent *, size_t>>
ComponentInfo::componentIdToIndexMap() const {
  return m_compIDToIndex;
}
} // namespace API
} // namespace Mantid
