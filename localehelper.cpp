#include "./localehelper.h"

#include <c++utilities/conversion/stringconversion.h>

#include <unordered_map>

using namespace std::literals;

namespace TagParser {

/// \cond
static const auto &languageMapping()
{
#include "resources/languages.h"
    return languages;
}
/// \endcond

/*!
 * \brief Returns the language name for the specified ISO-639-2 code (bibliographic, 639-2/B).
 * \remarks If \a isoCode is unknown an empty string is returned.
 */
const std::string &languageNameFromIso(const std::string &isoCode)
{
    const auto &mapping = languageMapping();
    const auto i = mapping.find(isoCode);
    if (i == mapping.cend()) {
        static const std::string empty;
        return empty;
    }
    return i->second;
}

/*!
 * \brief Returns the language name for the specified ISO-639-2 code (bibliographic, 639-2/B).
 * \remarks If \a isoCode is unknown the \a isoCode itself is returned.
 */
const std::string &languageNameFromIsoWithFallback(const std::string &isoCode)
{
    const auto &mapping = languageMapping();
    const auto i = mapping.find(isoCode);
    if (i == mapping.cend()) {
        return isoCode;
    }
    return i->second;
}

/*!
 * \brief Returns an empty LocaleDetail.
 */
const LocaleDetail &LocaleDetail::getEmpty()
{
    static const auto empty = LocaleDetail();
    return empty;
}

/*!
 * \brief Returns the abbreviated name of the specified \a format.
 *
 * This function returns the \a format if present. Otherwise it returns an empty string. This might be
 * improved in the future, e.g. to convert to the specified \a format. It would also be possible to combine
 * multiple details. For instance, if BCP-47 is required but only an ISO language and a domain name country
 * are present, both could be combined and returned as BCP-47 abbreviation.
 */
const LocaleDetail &Locale::abbreviatedName(LocaleFormat format) const
{
    for (const auto &detail : *this) {
        if (!detail.empty() && detail.format == format && isLanguageDefined(detail)) {
            return detail;
        }
    }
    return LocaleDetail::getEmpty();
}

/*!
 * \brief Returns *some* abbreviated name, *preferrably* of the specified \a preferredFormat.
 *
 * This function returns the \a preferredFormat if present. Otherwise it returns the most relevant
 * detail. This might be improved in the future, e.g. to convert to the \a preferredFormat. It would
 * also be possible to combine multiple details. For instance, if BCP-47 is preferred but only an ISO
 * language and a domain name country are present, both could be combined and returned as BCP-47 abbreviation.
 *
 * Returns an empty string if no details are present.
 *
 * \remarks This function is intended to be used for display purposes when the exact format doesn't matter and
 *          you just want to show the "best" abbreviation specified within the file.
 */
const LocaleDetail &Locale::someAbbreviatedName(LocaleFormat preferredFormat) const
{
    auto format = LocaleFormat::Unknown;
    const LocaleDetail *mostRelevantDetail = nullptr;
    for (const auto &detail : *this) {
        if (!detail.empty()
            && static_cast<std::underlying_type_t<LocaleFormat>>(detail.format) >= static_cast<std::underlying_type_t<LocaleFormat>>(format)) {
            if (detail.format == preferredFormat) {
                return detail;
            }
            format = detail.format;
            mostRelevantDetail = &detail;
        }
    }
    if (!mostRelevantDetail || !isLanguageDefined(*mostRelevantDetail)) {
        return LocaleDetail::getEmpty();
    }
    return *mostRelevantDetail;
}

/*!
 * \brief Returns the full name of the locale, e.g. Germany for the ISO code "ger" or an empty string if the
 *        full name is not known.
 * \remarks So far the full name is only known for ISO-639-2/B codes.
 */
const std::string &TagParser::Locale::fullName() const
{
    for (const auto &detail : *this) {
        if (detail.format == LocaleFormat::ISO_639_2_B || detail.format == LocaleFormat::ISO_639_2_T) {
            return languageNameFromIso(detail);
        }
    }
    return LocaleDetail::getEmpty();
}

/*!
 * \brief Returns the full name if possible and otherwise falls back to the abbreviated name.
 * \remarks This function is intended to be used for display purposes.
 */
const std::string &Locale::fullOrSomeAbbreviatedName() const
{
    if (const auto &name = fullName(); !name.empty()) {
        return name;
    }
    return someAbbreviatedName();
}

/*!
 * \brief Returns all details as comma-separated string.
 */
std::string Locale::toString() const
{
    return CppUtilities::joinStrings<std::vector<LocaleDetail>, std::string>(*this, LocaleDetail(", "sv, LocaleFormat::Unknown), true);
}

} // namespace TagParser
