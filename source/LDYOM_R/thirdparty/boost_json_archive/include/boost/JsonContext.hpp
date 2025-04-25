
#pragma once

#include <iostream>
#include <memory>
#include <stack>

// Boost Archive JSON
#include <boost/json.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

#include <memory>
#include <sstream>

#if defined(DEBUG_MODE) && !defined(ANDROID)
#include <iostream>
#endif

namespace param {
const std::string ClassNameType("class_name");
const std::string VersionType("version");
const std::string ItemVersionType("item_version");
const std::string ObjectIdType("object_id");
const std::string ObjectReferenceType("object_id_ref");
const std::string ClassIdType("class_id");
const std::string ClassIdOptionalType("class_id_opt");
const std::string ClassIdReferenceType("class_id_ref");
const std::string TrackingType("tracking");
} // namespace param

#ifdef __GNUG__

std::string BOOST_SYMBOL_EXPORT demangle(const char *name);

#else
// does nothing if not g++
std::string BOOST_SYMBOL_EXPORT demangle(const char *name);
#endif

/**
 * @brief JsonContext Class. Helper for boost::serialization::json.
 */
class BOOST_SYMBOL_EXPORT JsonContext {

public:
  typedef std::shared_ptr<boost::json::value> json_value;
  typedef std::stack<std::pair<std::string, json_value>>::value_type stack_value_type;

  /**
   * @brief Get raw value depending on T type (and boost::json::kind for integral types).
   * @tparam T
   * @param json_value
   * @return T
   */
  template <typename T> static T get(boost::json::value &json_value) {
    if constexpr (std::is_same<T, bool>::value) {
      return json_value.get_bool();
    } else if constexpr (std::is_integral<T>::value) {
      if (json_value.kind() == boost::json::kind::int64) {
        return json_value.get_int64();
      } else {
        return json_value.get_uint64();
      }
    } else if constexpr (std::is_floating_point<T>::value) {
      return json_value.get_double();
    } else if constexpr (std::is_same<T, std::string>::value) {
      return json_value.get_string().c_str();
    }
    throw std::runtime_error("Non-handled Json type !");
  }

  /**
   * @brief Emplace raw value depending on T type.
   * @tparam T
   * @param json_value
   * @return T
   */
  template <typename T> static void emplace(boost::json::value &json_value, const T val) {
    if constexpr (std::is_same<T, bool>::value) {
      json_value.emplace_bool();
      json_value.get_bool() = val;
    } else if constexpr (std::is_integral<T>::value && std::is_signed<T>::value) {
      json_value.emplace_int64();
      json_value.get_int64() = val;
    } else if constexpr (std::is_integral<T>::value && std::is_unsigned<T>::value) {
      json_value.emplace_uint64();
      json_value.get_uint64() = val;
    } else if constexpr (std::is_floating_point<T>::value) {
      json_value.emplace_double();
      json_value.get_double() = val;
    } else if constexpr (std::is_same<T, std::string>::value) {
      json_value.emplace_string();
      json_value.get_string() = val;
    } else {
      throw std::runtime_error("Non-handled Json type !");
    }
  }

private:
  /**
   * @brief Json Stack context.
   */
  std::stack<std::pair<std::string, json_value>> m_context;
  /**
   * @brief Root Json Value.
   */
  std::shared_ptr<boost::json::value> m_root;
  /**
   * @brief Current Stack value.
   */
  stack_value_type m_current;
  /**
   * @brief Currently handled tag (can differe from m_current.first).
   */
  std::string m_current_tag;

public:
  /**
   * @brief Parse Input Stream to Json.
   * @param is
   * @param ec
   * @param sp
   * @param opt
   * @return boost::json::value
   */
  static boost::json::value parse(std::istream &is, boost::system::error_code &ec, boost::json::storage_ptr sp = {},
                                  boost::json::parse_options const &opt = {}) {
    try {
      std::string s(std::istreambuf_iterator<char>(is), {});
      boost::json::value jv = boost::json::parse(s, ec, sp, opt);
      return jv;
    } catch (std::exception const &e) {
#if defined(DEBUG_MODE) && !defined(ANDROID)
      std::cout << "Parsing failed: " << e.what() << "\n";
#endif
    }
    return {};
  }

  /**
   * @brief Construct a new Json Context.
   * @param m_root
   */
  explicit JsonContext(std::shared_ptr<boost::json::value> m_root);
  /**
   * @brief Construct a new Json Context.
   */
  JsonContext();

  /**
   * @brief Return true if the context stack is empty.
   * @return true
   * @return false
   */
  bool empty() const;
  /**
   * @brief Push to context stack.
   * @param tag
   * @param value
   * @param setCurrent
   */
  void push(std::string tag, std::shared_ptr<boost::json::value> value, bool setCurrent = true);
  /**
   * @brief Pop from context stack.
   */
  void pop();
  /**
   * @brief Context stack size.
   * @return size_t
   */
  size_t size() { return m_context.size(); }
  /**
   * @brief Return the top of the context stack.
   * @return stack_value_type&
   */
  stack_value_type &top();
  /**
   * @brief Return the root value of the context stack.
   * @return const std::shared_ptr<boost::json::value>&
   */
  const std::shared_ptr<boost::json::value> &root();
  /**
   * @brief Set the Root of the context stack.
   * @param tag
   * @param v
   */
  void setRoot(std::string tag, std::shared_ptr<boost::json::value> v);
  /**
   * @brief Get currently handled stack value.
   * @return stack_value_type&
   */
  stack_value_type &current();
  /**
   * @brief Set the currently handled stack value.
   * @param tag
   * @param v
   */
  void setCurrent(std::string tag, std::shared_ptr<boost::json::value> &v);
  /**
   * @brief Set the currently processed tag.
   * @param tag
   */
  void setCurrentTag(std::string tag);
  /**
   * @brief Get the currently processed tag.
   * @return std::string
   */
  std::string currentTag();
  /**
   * @brief Serialize root as Json to output stream.
   * @param os
   * @param prettify
   */
  void serialize(std::ostream &os, bool prettify = false);
};
