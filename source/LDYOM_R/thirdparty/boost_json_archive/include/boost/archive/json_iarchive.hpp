#ifndef BOOST_JSON_ARCHIVE_IARCHIVE_H
#define BOOST_JSON_ARCHIVE_IARCHIVE_H

// C++ Standard Library
#include <istream>
#include <iterator>
#include <sstream>

// Boost
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

// Boost Archive JSON
#include "boost/JsonContext.hpp"
#include "boost/archive/TraitsDetailsHelper.hpp"

namespace boost {
	namespace archive {
		class BOOST_SYMBOL_EXPORT json_iarchive : public detail::common_iarchive<json_iarchive> {
		public:
			explicit json_iarchive(std::istream &is, unsigned int = 0);

			~json_iarchive() override = default;

			template <typename T>
			std::enable_if_t<std::is_fundamental<T>::type::value || std::is_same<T, std::string>::type::value>
			load_fundamental(T &value) {
				if (m_ctx.current().second->is_object() && !m_ctx.current().second->get_object()[m_ctx.currentTag().
					c_str()].is_null()) {
					value = JsonContext::get<T>(m_ctx.current().second->get_object()[m_ctx.currentTag().c_str()]);
				} else if (!m_ctx.current().second->is_null()) {
					value = JsonContext::get<T>(*m_ctx.current().second);
				} else {
					throw std::runtime_error("Json value not found !");
				}
			}

			template <typename T>
			void load_smart_ptr(T &value) {
				typename T::element_type *t = nullptr;
				common_iarchive<json_iarchive>::load_override(t);
				if constexpr (detail::is_shared_ptr<T>::value || detail::is_weak_ptr<T>::value) {
					if (_shared_hack.contains(static_cast<uint64_t>(t))) {
						value = static_pointer_cast<typename T::element_type>(_shared_hack[static_cast<uint64_t>(t)]);
					} else {
						if constexpr (detail::is_weak_ptr<T>::value) {
							auto sptr = std::shared_ptr<typename T::element_type>(t);
							value = sptr;
							_shared_hack[static_cast<uint64_t>(t)] = reinterpret_pointer_cast<void>(
								const_pointer_cast<std::remove_const_t<typename T::element_type>>(sptr));
						} else {
							value.reset(t);
							_shared_hack[static_cast<uint64_t>(t)] = reinterpret_pointer_cast<void>(
								const_pointer_cast<std::remove_const_t<typename T::element_type>>(value));
						}
					}
				} else {
					value.reset(t);
				}
			}

			template <typename T>
			void load(std::vector<T> &value) {
				size_t current_capacity = value.capacity();
				value.clear();
				value.reserve(current_capacity);
				system::error_code ec;
				json::value *pxp = nullptr;
				if (!m_ctx.top().second->is_array()) {
					m_ctx.pop();
					pxp = m_ctx.top().second->find_pointer("/px/" + m_ctx.currentTag(), ec);
				}
				if (m_ctx.top().second->is_array() || (pxp && !ec)) {
					auto &array = pxp ? pxp->get_array() : m_ctx.top().second->get_array();
					unsigned int index = 0;
					for (auto &val : array) {
						if constexpr ((std::is_class_v<T> && !std::is_same_v<T, std::string>) || std::is_pointer_v<T>) {
							m_ctx.push(std::to_string(index), std::make_shared<json::value>(val));

							T _val;
							load(_val);
							if constexpr (detail::is_unique_ptr<T>::value) {
								value.push_back(std::move(_val));
							} else {
								value.push_back(_val);
							}

							m_ctx.pop();
						} else {
							value.push_back(JsonContext::get<T>(val));
						}
						index++;
					}
				}
			}

			template <typename T>
			void load(T &value) {
				if constexpr (std::is_fundamental_v<T> || std::is_same_v<T, std::string>) {
					load_fundamental(value);
				} else if constexpr (std::is_enum_v<T>) {
					int64_t converted = -1;
					load_fundamental(converted);
					value = static_cast<T>(converted);
				} else if constexpr (detail::is_fixed_size_old_school_array<T>::value) {
					std::array<std::remove_all_extents_t<T>, detail::is_fixed_size_old_school_array<T>::size> arr;
					load(arr);
					std::copy(arr.begin(), arr.end(), std::begin(value));
				} else if constexpr (detail::is_fixed_size_array<T>::value) {
					std::vector<typename T::value_type> vec;
					load(vec);
					std::copy(vec.begin(), vec.end(), std::begin(value));
				} else if constexpr (detail::is_std_map<T>::value) {
					std::vector<std::pair<typename T::key_type, typename T::mapped_type>> vec;
					load(vec);
					std::transform(vec.begin(), vec.end(), std::inserter(value, value.end()),
					               [](const auto &pair) { return std::make_pair(pair.first, pair.second); });
				} else if constexpr (detail::is_shared_ptr<T>::value || detail::is_unique_ptr<T>::value ||
					detail::is_weak_ptr<T>::value) {
					load_smart_ptr<T>(value);
				} else {
					common_iarchive<json_iarchive>::load_override(value);
				}
			}

			/*************************************************************************
			 *                              NVP                                      *
			 *************************************************************************/
			template <typename T>
			void load_override(const nvp<T> &nvp) {
				size_t ctx_size = m_ctx.size();
				if (ctx_size == 0) {
					m_ctx.setRoot(root_value.is_object() && !root_value.as_object().empty() ? nvp.name() : "",
					              std::make_shared<json::value>(root_value));
				}
				auto &top_value = m_ctx.top();
				bool pushed = false;
				bool pxed = false;
				if (nvp.name() && top_value.second->is_object()) {
					json::value *ptr = nullptr;
					if (m_px_level > 0) {
						system::error_code ec;
						ptr = top_value.second->find_pointer("/px/" + std::string(nvp.name()), ec);
					}
					if (m_px_level > 0 && (ptr != nullptr)) {
						m_ctx.push(nvp.name(), std::make_shared<json::value>(*ptr));
					} else {
						m_ctx.push(nvp.name(),
						           std::make_shared<json::value>(top_value.second->get_object()[nvp.name()]));
					}
					pushed = true;
					ctx_size = m_ctx.size();
					m_ctx.setCurrent(nvp.name(), m_ctx.top().second);
				} else {
					auto key = "px";
					m_ctx.setCurrentTag(key);
					m_px_level++;
					pxed = true;
				}
				this->load(nvp.value());
				if (pushed && m_ctx.size() > 1 && ctx_size == m_ctx.size()) {
					m_ctx.pop();
				}
				if (pxed) {
					m_px_level--;
				}
			}

			void load_binary(void *t, std::size_t size) { }

			/*************************************************************************
			 *                          METATYPES                                    *
			 *************************************************************************/
			void load_override(class_name_type &t);
			void load_override(version_type &t);
			void load_override(object_id_type &t);
			void load_override(object_reference_type &t);
			void load_override(class_id_type &t);
			void load_override(class_id_optional_type &t);
			void load_override(class_id_reference_type &t);
			void load_override(tracking_type &t);

		private:
			/**
			 * @brief Json Root Value.
			 */
			json::value root_value;
			/**
			 * @brief JsonContext. Using a stack to know the current "location in the Json tree".
			 */
			JsonContext m_ctx;
			/**
			 * @brief Current "pointer level". To know if we have to check for values under "/px/" nodes.
			 */
			uint32_t m_px_level = 0;
			/**
			 * @brief Storage for shared pointers (std::shared_ptr).
			 */
			std::unordered_map<int64_t, std::shared_ptr<void>> _shared_hack;
		};
	} // namespace archive
} // namespace boost

BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::json_iarchive)

#endif // BOOST_JSON_ARCHIVE_IARCHIVE_H
