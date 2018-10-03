#include "ParseReflectometryStrings.h"
#include "../Parse.h"
#include "AllInitialized.h"
#include "MantidKernel/Strings.h"
#include "MantidQtWidgets/Common/ParseKeyValueString.h"
#include <set>
namespace MantidQt {
namespace CustomInterfaces {

boost::optional<std::string>
parseRunNumber(std::string const &runNumberString) {
  auto asInt = parseNonNegativeInt(std::move(runNumberString));
  if (asInt.is_initialized()) {
    auto withoutWhitespaceAndDefinatelyPositive = std::to_string(asInt.get());
    return withoutWhitespaceAndDefinatelyPositive;
  } else {
    return boost::none;
  }
}

boost::optional<std::string>
parseRunNumberOrWhitespace(std::string const &runNumberString) {
  if (isEntirelyWhitespace(runNumberString)) {
    return std::string();
  } else {
    auto maybeRunNumber = parseRunNumber(runNumberString);
    if (maybeRunNumber.is_initialized())
      return maybeRunNumber;
  }
  return boost::none;
}

boost::optional<double> parseTheta(std::string const &theta) {
  auto maybeTheta = parseNonNegativeDouble(std::move(theta));
  if (maybeTheta.is_initialized() && maybeTheta.get() > 0.0)
    return maybeTheta;
  else
    return boost::none;
}

boost::optional<std::map<std::string, std::string>>
parseOptions(std::string const &options) {
  try {
    return MantidQt::MantidWidgets::parseKeyValueString(options);
  } catch (std::runtime_error &) {
    return boost::none;
  }
}

boost::optional<boost::optional<std::string>>
parseProcessingInstructions(std::string const &instructions) {
  if (isEntirelyWhitespace(instructions)) {
    return boost::optional<std::string>(boost::none);
  } else {
    try {
      auto const groups =
          Mantid::Kernel::Strings::parseGroups<size_t>(instructions);
      return boost::optional<std::string>(instructions);
    } catch (std::runtime_error &) {
      return boost::none;
    }
  }
  return boost::none;
}

boost::optional<boost::optional<double>>
parseScaleFactor(std::string const &scaleFactor) {
  if (isEntirelyWhitespace(scaleFactor)) {
    return boost::optional<double>(boost::none);
  } else {
    auto value = parseDouble(scaleFactor);
    if (value.is_initialized())
      return value;
  }
  return boost::none;
}

boost::variant<RangeInQ, std::vector<int>>
parseQRange(std::string const &min, std::string const &max,
            std::string const &step) {
  auto invalidParams = std::vector<int>();
  boost::optional<double> minimum;
  boost::optional<double> maximum;
  boost::optional<double> stepValue;

  // If any values are set, check they parse ok
  if (!isEntirelyWhitespace(min)) {
    minimum = parseNonNegativeDouble(min);
    if (!minimum.is_initialized())
      invalidParams.emplace_back(0);
  }

  if (!isEntirelyWhitespace(max)) {
    maximum = parseNonNegativeDouble(max);
    if (!maximum.is_initialized())
      invalidParams.emplace_back(1);
  }

  if (!isEntirelyWhitespace(step)) {
    stepValue = parseNonNegativeDouble(step);
    if (!stepValue.is_initialized())
      invalidParams.emplace_back(2);
  }

  // Check max is not less than min
  if (maximum.is_initialized() && minimum.is_initialized() &&
      maximum.get() < minimum.get()) {
    invalidParams.emplace_back(0);
    invalidParams.emplace_back(1);
  }

  // Return errors, valid range, or unset if nothing was specified
  if (!invalidParams.empty())
    return invalidParams;
  else
    return RangeInQ(minimum, stepValue, maximum);
}

boost::optional<std::vector<std::string>>
parseRunNumbers(std::string const &runNumberString) {
  auto runNumbers = std::vector<std::string>();
  auto runNumberCandidates =
      boost::tokenizer<boost::escaped_list_separator<char>>(
          runNumberString,
          boost::escaped_list_separator<char>("\\", ",+", "\"'"));

  for (auto const &runNumberCandidate : runNumberCandidates) {
    auto maybeRunNumber = parseRunNumber(runNumberCandidate);
    if (maybeRunNumber.is_initialized()) {
      runNumbers.emplace_back(maybeRunNumber.get());
    } else {
      return boost::none;
    }
  }

  if (runNumbers.empty())
    return boost::none;
  else
    return runNumbers;
}

boost::variant<std::pair<std::string, std::string>, std::vector<int>>
parseTransmissionRuns(std::string const &firstTransmissionRun,
                      std::string const &secondTransmissionRun) {
  auto errorColumns = std::vector<int>();
  auto first = parseRunNumberOrWhitespace(firstTransmissionRun);
  auto second = parseRunNumberOrWhitespace(secondTransmissionRun);

  if (allInitialized(first, second)) {
    if (first.get().empty() && !second.get().empty()) {
      errorColumns.emplace_back(0);
      return errorColumns;
    } else {
      return TransmissionRunPair(first.get(), second.get());
    }
  } else {
    if (!first.is_initialized())
      errorColumns.emplace_back(0);
    if (!second.is_initialized())
      errorColumns.emplace_back(1);
    return errorColumns;
  }
}

} // namespace CustomInterfaces
} // namespace MantidQt
