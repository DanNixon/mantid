#ifndef MANTID_PARALLEL_EVENTLOADER_H_
#define MANTID_PARALLEL_EVENTLOADER_H_

#include <memory>
#include <string>
#include <vector>

#include "MantidParallel/DllConfig.h"

namespace Mantid {
class TofEvent;
namespace Parallel {
namespace IO {
class Chunker;
template <class IndexType, class TimeZeroType, class TimeOffsetType>
class NXEventDataSource;

/** Loader for event data from Nexus files with parallelism based on multiple
  processes (MPI) for performance.

  @author Simon Heybrock
  @date 2017

  Copyright &copy; 2017 ISIS Rutherford Appleton Laboratory, NScD Oak Ridge
  National Laboratory & European Spallation Source

  This file is part of Mantid.

  Mantid is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Mantid is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  File change history is stored at: <https://github.com/mantidproject/mantid>
  Code Documentation is available at: <http://doxygen.mantidproject.org>
*/
namespace EventLoader {
MANTID_PARALLEL_DLL void load(const std::string &filename,
                              const std::string &groupName,
                              const std::vector<std::string> &bankNames,
                              const std::vector<int32_t> &bankOffsets,
                              std::vector<std::vector<TofEvent> *> eventLists);

namespace detail {
template <class IndexType, class TimeZeroType, class TimeOffsetType>
void load(
    const Chunker &chunker,
    NXEventDataSource<IndexType, TimeZeroType, TimeOffsetType> &dataSource);

template <class IndexType, class TimeZeroType, class TimeOffsetType>
void load(const H5::Group &group, const std::vector<std::string> &bankNames,
          const std::vector<int32_t> &bankOffsets,
          std::vector<std::vector<TofEvent> *> eventLists);

template <class... T1, class... T2>
void load(const H5::DataType &type, T2 &&... args) {
  // Translate from H5::DataType to actual type. Done step by step to avoid
  // combinatoric explosion. The T1 parameter pack holds the final template
  // arguments we want. The T2 parameter pack represents the remaining
  // H5::DataType arguments and any other arguments. In every call we peel off
  // the first entry from the T2 pack and append it to T1. This stops once the
  // next argument in args is not of type H5::DataType anymore, allowing us to
  // pass arbitrary extra arguments in the second part of args.
  if (type == H5::PredType::NATIVE_INT32)
    return load<T1..., int32_t>(args...);
  if (type == H5::PredType::NATIVE_INT64)
    return load<T1..., int64_t>(args...);
  if (type == H5::PredType::NATIVE_UINT32)
    return load<T1..., uint32_t>(args...);
  if (type == H5::PredType::NATIVE_UINT64)
    return load<T1..., uint64_t>(args...);
  if (type == H5::PredType::NATIVE_FLOAT)
    return load<T1..., float>(args...);
  if (type == H5::PredType::NATIVE_DOUBLE)
    return load<T1..., double>(args...);
  throw std::runtime_error(
      "Unsupported H5::DataType for entry in NXevent_data");
}
}
}

} // namespace IO
} // namespace Parallel
} // namespace Mantid

#endif /* MANTID_PARALLEL_EVENTLOADER_H_ */
