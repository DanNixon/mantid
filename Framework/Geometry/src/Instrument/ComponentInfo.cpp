#include "MantidGeometry/Instrument/ComponentInfo.h"
#include "MantidGeometry/Objects/BoundingBox.h"
#include "MantidGeometry/Objects/Object.h"
#include "MantidGeometry/IComponent.h"
#include "MantidBeamline/ComponentInfo.h"
#include "MantidKernel/EigenConversionHelpers.h"
#include "MantidKernel/Exception.h"
#include "MantidKernel/make_unique.h"
#include <exception>
#include <string>
#include <Eigen/Geometry>
#include <stack>
#include <iterator>

namespace Mantid {
namespace Geometry {

namespace {
/**
 * Rotate point by inverse of rotation held at componentIndex
 */
const Eigen::Vector3d undoRotation(const Eigen::Vector3d &point,
                                   const Beamline::ComponentInfo &compInfo,
                                   const size_t componentIndex) {
  auto unRotateTransform =
      Eigen::Affine3d(compInfo.rotation(componentIndex).inverse());
  return unRotateTransform * point;
}
/**
 * Put the point into the frame of the shape.
 * 1. Subtract component position (puts component pos at origin, same as shape
 * coordinate system).
 * 2. Apply inverse rotation of component to point. Unrotates the component into
 * shape coordinate frame, with observer reorientated.
 */
const Kernel::V3D toShapeFrame(const Kernel::V3D &point,
                               const Beamline::ComponentInfo &compInfo,
                               const size_t componentIndex) {
  return Kernel::toV3D(undoRotation(Kernel::toVector3d(point) -
                                        compInfo.position(componentIndex),
                                    compInfo, componentIndex));
}
}

/**
 * Constructor.
 * @param componentInfo : Internal Beamline ComponentInfo
 * @param componentIds : ComponentIDs ordered by component
 * @param componentIdToIndexMap : ID -> index translation map
 * @param shapes : Shapes for each component
 * */
ComponentInfo::ComponentInfo(
    std::unique_ptr<Beamline::ComponentInfo> componentInfo,
    boost::shared_ptr<const std::vector<Mantid::Geometry::IComponent *>>
        componentIds,
    boost::shared_ptr<const std::unordered_map<Geometry::IComponent *, size_t>>
        componentIdToIndexMap,
    boost::shared_ptr<std::vector<boost::shared_ptr<const Geometry::Object>>>
        shapes)
    : m_componentInfo(std::move(componentInfo)),
      m_componentIds(std::move(componentIds)),
      m_compIDToIndex(std::move(componentIdToIndexMap)),
      m_shapes(std::move(shapes)) {

  if (m_componentIds->size() != m_compIDToIndex->size()) {
    throw std::invalid_argument("Inconsistent ID and Mapping input containers "
                                "for Geometry::ComponentInfo");
  }
  if (m_componentIds->size() != m_componentInfo->size()) {
    throw std::invalid_argument("Inconsistent ID and base "
                                "Beamline::ComponentInfo sizes for "
                                "Geometry::ComponentInfo");
  }
}

/** Copy constructor. Use with EXTREME CARE.
 *
 * Public copy should not be used since proper links between DetectorInfo and
 * ComponentInfo must be set up. */
ComponentInfo::ComponentInfo(const ComponentInfo &other)
    : m_componentInfo(
          Kernel::make_unique<Beamline::ComponentInfo>(*other.m_componentInfo)),
      m_componentIds(other.m_componentIds),
      m_compIDToIndex(other.m_compIDToIndex), m_shapes(other.m_shapes) {}

// Defined as default in source for forward declaration with std::unique_ptr.
ComponentInfo::~ComponentInfo() = default;

std::vector<size_t>
ComponentInfo::detectorsInSubtree(size_t componentIndex) const {
  return m_componentInfo->detectorsInSubtree(componentIndex);
}

std::vector<size_t>
ComponentInfo::componentsInSubtree(size_t componentIndex) const {
  return m_componentInfo->componentsInSubtree(componentIndex);
}

size_t ComponentInfo::size() const { return m_componentInfo->size(); }

size_t ComponentInfo::indexOf(Geometry::IComponent *id) const {
  return m_compIDToIndex->at(id);
}

bool ComponentInfo::isDetector(const size_t componentIndex) const {
  return m_componentInfo->isDetector(componentIndex);
}

Kernel::V3D ComponentInfo::position(const size_t componentIndex) const {
  return Kernel::toV3D(m_componentInfo->position(componentIndex));
}

Kernel::Quat ComponentInfo::rotation(const size_t componentIndex) const {
  return Kernel::toQuat(m_componentInfo->rotation(componentIndex));
}

Kernel::V3D ComponentInfo::relativePosition(const size_t componentIndex) const {
  return Kernel::toV3D(m_componentInfo->relativePosition(componentIndex));
}

Kernel::Quat
ComponentInfo::relativeRotation(const size_t componentIndex) const {
  return Kernel::toQuat(m_componentInfo->relativeRotation(componentIndex));
}

size_t ComponentInfo::parent(const size_t componentIndex) const {
  return m_componentInfo->parent(componentIndex);
}

bool ComponentInfo::hasParent(const size_t componentIndex) const {
  return m_componentInfo->hasParent(componentIndex);
}

bool ComponentInfo::hasShape(const size_t componentIndex) const {
  return (*m_shapes)[componentIndex].get() != nullptr;
}

Kernel::V3D ComponentInfo::sourcePosition() const {
  return Kernel::toV3D(m_componentInfo->sourcePosition());
}

Kernel::V3D ComponentInfo::samplePosition() const {
  return Kernel::toV3D(m_componentInfo->samplePosition());
}

bool ComponentInfo::hasSource() const { return m_componentInfo->hasSource(); }

bool ComponentInfo::hasSample() const { return m_componentInfo->hasSample(); }

size_t ComponentInfo::source() const { return m_componentInfo->source(); }

size_t ComponentInfo::sample() const { return m_componentInfo->sample(); }

double ComponentInfo::l1() const { return m_componentInfo->l1(); }

size_t ComponentInfo::root() { return m_componentInfo->root(); }

void ComponentInfo::setPosition(const size_t componentIndex,
                                const Kernel::V3D &newPosition) {
  m_componentInfo->setPosition(componentIndex, Kernel::toVector3d(newPosition));
}

void ComponentInfo::setRotation(const size_t componentIndex,
                                const Kernel::Quat &newRotation) {
  m_componentInfo->setRotation(componentIndex,
                               Kernel::toQuaterniond(newRotation));
}

const Object &ComponentInfo::shape(const size_t componentIndex) const {
  return *(*m_shapes)[componentIndex];
}

Kernel::V3D ComponentInfo::scaleFactor(const size_t componentIndex) const {
  return Kernel::toV3D(m_componentInfo->scaleFactor(componentIndex));
}

void ComponentInfo::setScaleFactor(const size_t componentIndex,
                                   const Kernel::V3D &scaleFactor) {
  m_componentInfo->setScaleFactor(componentIndex,
                                  Kernel::toVector3d(scaleFactor));
}

double ComponentInfo::solidAngle(const size_t componentIndex,
                                 const Kernel::V3D &observer) const {
  if (!hasShape(componentIndex))
    throw Kernel::Exception::NullPointerException("ComponentInfo::solidAngle",
                                                  "shape");
  // This is the observer position in the shape's coordinate system.
  const Kernel::V3D relativeObserver =
      toShapeFrame(observer, *m_componentInfo, componentIndex);
  const Kernel::V3D scaleFactor = this->scaleFactor(componentIndex);
  if ((scaleFactor - Kernel::V3D(1.0, 1.0, 1.0)).norm() < 1e-12)
    return shape(componentIndex).solidAngle(relativeObserver);
  else {
    // This function will scale the object shape when calculating the solid
    // angle.
    return shape(componentIndex).solidAngle(relativeObserver, scaleFactor);
  }
}

/**
 * The absolute bounding box is cleared/reassigned as part of this.
 * The absoluteBB is not grown.
 *
 * @param index : Component index
 * @param absoluteBB : Absolute bounding box. This is rewritten.
 */
void ComponentInfo::componentBoundingBox(const size_t index,
                                         BoundingBox &absoluteBB) const {
  // Check that we have a valid shape here
  if (!hasShape(index)) {
    absoluteBB.nullify();
    return; // This index will not contribute to bounding box
  }
  const auto &s = this->shape(index);
  const BoundingBox &shapeBox = s.getBoundingBox();

  std::vector<Kernel::V3D> coordSystem;
  if (!absoluteBB.isAxisAligned()) { // copy coordinate system (it is better

    coordSystem.assign(absoluteBB.getCoordSystem().begin(),
                       absoluteBB.getCoordSystem().end());
  }
  absoluteBB = BoundingBox(shapeBox);
  // modify in place for speed
  const Eigen::Vector3d scaleFactor = m_componentInfo->scaleFactor(index);
  // Scale
  absoluteBB.xMin() *= scaleFactor[0];
  absoluteBB.xMax() *= scaleFactor[0];
  absoluteBB.yMin() *= scaleFactor[1];
  absoluteBB.yMax() *= scaleFactor[1];
  absoluteBB.zMin() *= scaleFactor[2];
  absoluteBB.zMax() *= scaleFactor[2];
  // Rotate
  (this->rotation(index))
      .rotateBB(absoluteBB.xMin(), absoluteBB.yMin(), absoluteBB.zMin(),
                absoluteBB.xMax(), absoluteBB.yMax(), absoluteBB.zMax());

  // Shift
  const Eigen::Vector3d localPos = m_componentInfo->position(index);
  absoluteBB.xMin() += localPos[0];
  absoluteBB.xMax() += localPos[0];
  absoluteBB.yMin() += localPos[1];
  absoluteBB.yMax() += localPos[1];
  absoluteBB.zMin() += localPos[2];
  absoluteBB.zMax() += localPos[2];

  if (!coordSystem.empty()) {
    absoluteBB.realign(&coordSystem);
  }
}

void ComponentInfo::getBoundingBox(const size_t componentIndex,
                                   BoundingBox &absoluteBB) const {

  if (isDetector(componentIndex)) {
    componentBoundingBox(componentIndex, absoluteBB);
    return;
  }
  auto rangeComp = m_componentInfo->componentRangeInSubtree(componentIndex);
  std::stack<std::pair<size_t, size_t>> detExclusions{};
  auto compIterator = rangeComp.rbegin();
  while (compIterator != rangeComp.rend()) {
    BoundingBox temp;
    const size_t index = *compIterator;
    if (hasSource() && index == source()) {
      ++compIterator;
    } else if (isRectangularBank(index)) {
      auto innerRangeComp = m_componentInfo->componentRangeInSubtree(index);
      // nSubComponents, subtract off self hence -1.
      auto nSubComponents = innerRangeComp.end() - innerRangeComp.begin() - 1;
      auto innerRangeDet = m_componentInfo->detectorRangeInSubtree(index);
      auto nSubDetectors =
          std::distance(innerRangeDet.begin(), innerRangeDet.end());
      auto nY = nSubDetectors / nSubComponents;
      size_t corner1 = *innerRangeDet.begin();
      size_t corner2 = corner1 + nSubDetectors - 1;
      size_t corner3 = corner1 + (nY - 1);
      size_t corner4 = corner2 - (nY - 1);

      componentBoundingBox(corner1, temp);
      absoluteBB.grow(temp);
      componentBoundingBox(corner2, temp);
      absoluteBB.grow(temp);
      componentBoundingBox(corner3, temp);
      absoluteBB.grow(temp);
      componentBoundingBox(corner4, temp);
      absoluteBB.grow(temp);

      // Get bounding box for rectangular bank.
      // Record detector ranges to skip
      // Skip all sub components.
      detExclusions.emplace(std::make_pair(corner1, corner2));
      compIterator = innerRangeComp.rend();
    } else {
      componentBoundingBox(index, temp);
      absoluteBB.grow(temp);
      ++compIterator;
    }
  }

  // Now deal with bounding boxes for detectors
  auto rangeDet = m_componentInfo->detectorRangeInSubtree(componentIndex);
  auto detIterator = rangeDet.begin();
  auto *exclusion = detExclusions.empty() ? nullptr : &detExclusions.top();
  while (detIterator != rangeDet.end()) {

    // Handle detectors in exclusion ranges
    if (exclusion && (*detIterator) >= exclusion->first &&
        (*detIterator) <= exclusion->second) {
      detIterator += (exclusion->second - exclusion->first +
                      1); // Jump the iterator forward
      detExclusions.pop();
      exclusion = detExclusions.empty() ? nullptr : &detExclusions.top();
    } else if (detIterator != rangeDet.end()) {
      BoundingBox temp;
      componentBoundingBox(*detIterator, temp);
      absoluteBB.grow(temp);
      ++detIterator;
    }
  }
}

bool ComponentInfo::isRectangularBank(const size_t componentIndex) const {
  return m_componentInfo->isRectangularBank(componentIndex);
}

} // namespace Geometry
} // namespace Mantid
