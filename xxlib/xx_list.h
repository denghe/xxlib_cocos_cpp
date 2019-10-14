#pragma once
#include "xx_object.h"

namespace xx
{
	struct BBuffer;

	// ���� std vector / .net List �ļ�����
	// reservedHeaderLen Ϊ���� buf �ڴ����ǰ��ճ�һ���ڴ治��, Ҳ����ʼ��, ���ݲ�����( Ϊ����ͷ�����ݴ������ )
	template<typename T, std::size_t reservedHeaderLen = 0>
	struct List : Object {
		typedef T ChildType;
		T* buf;
		std::size_t		cap;
		std::size_t		len;

		List() noexcept
			: buf(nullptr)
			, cap(0)
			, len(0) {
		}

		explicit List(std::size_t const& cap) noexcept {
			if (cap == 0) {
				buf = nullptr;
				this->cap = 0;
			}
			else {
				// ������� 2^n �ռ�
				auto bufByteLen = Round2n((reservedHeaderLen + cap) * sizeof(T));
				buf = (T*)::malloc(bufByteLen) + reservedHeaderLen;
				this->cap = std::size_t(bufByteLen / sizeof(T) - reservedHeaderLen);
			}
			len = 0;
		}

		List(List&& o) noexcept
			: buf(o.buf)
			, cap(o.cap)
			, len(o.len) {
			o.buf = nullptr;
			o.cap = 0;
			o.len = 0;
		}

		List& operator=(List&& o) noexcept {
			std::swap(buf, o.buf);
			std::swap(len, o.len);
			std::swap(cap, o.cap);
			return *this;
		}

		~List() noexcept {
			Clear(true);
		}

		List(List const& o) = delete;
		List& operator=(List const& o) = delete;

		void Reserve(std::size_t const& cap) noexcept {
			if (cap <= this->cap) return;

			auto newBufByteLen = Round2n((reservedHeaderLen + cap) * sizeof(T));
			auto newBuf = (T*)::malloc(newBufByteLen) + reservedHeaderLen;

			if constexpr (IsTrivial_v<T>) {
				::memcpy((void*)newBuf, (void*)buf, len * sizeof(T));
			}
			else {
				for (std::size_t i = 0; i < len; ++i) {
					new (&newBuf[i]) T((T&&)buf[i]);
					buf[i].~T();
				}
			}

			if (buf) ::free(buf - reservedHeaderLen);
			buf = newBuf;
			this->cap = std::size_t(newBufByteLen / sizeof(T) - reservedHeaderLen);
		}

		std::size_t Resize(std::size_t const& len) noexcept {
			if (len == this->len) return len;
			else if (len < this->len) {
				if constexpr (!std::is_pod_v<T>) {
					for (std::size_t i = len; i < this->len; ++i) {
						buf[i].~T();
					}
				}
			}
			else {	// len > this->len
				Reserve(len);
				if constexpr (!std::is_pod_v<T>) {
					for (std::size_t i = this->len; i < len; ++i) {
						new (buf + i) T();
					}
				}
			}
			auto rtv = this->len;
			this->len = len;
			return rtv;
		}

		T const& operator[](std::size_t const& idx) const noexcept {
			assert(idx < len);
			return buf[idx];
		}
		T& operator[](std::size_t const& idx) noexcept {
			assert(idx < len);
			return buf[idx];
		}
		T const& At(std::size_t const& idx) const noexcept {
			assert(idx < len);
			return buf[idx];
		}
		T& At(std::size_t const& idx) noexcept {
			assert(idx < len);
			return buf[idx];
		}

		T& Top() noexcept {
			assert(len > 0);
			return buf[len - 1];
		}
		void Pop() noexcept {
			assert(len > 0);
			--len;
			buf[len].~T();
		}
		T const& Top() const noexcept {
			assert(len > 0);
			return buf[len - 1];
		}
		bool TryPop(T& output) noexcept {
			if (!len) return false;
			output = (T&&)buf[--len];
			buf[len].~T();
			return true;
		}

		void Clear(bool const& freeBuf = false) noexcept {
			if (!buf) return;
			if (len) {
				if constexpr (!std::is_pod_v<T>) {
					for (std::size_t i = len - 1; i != (std::size_t) - 1; --i) {
						buf[i].~T();
					}
				}
				len = 0;
			}
			if (freeBuf) {
				::free(buf - reservedHeaderLen);
				buf = nullptr;
				cap = 0;
			}
		}

		// unsafe: direct change field value
		void Reset(uint8_t* const& buf = nullptr, std::size_t const& len = 0, std::size_t const& cap = 0) noexcept {
			this->buf = buf;
			this->len = len;
			this->cap = cap;
		}

		void Remove(T const& v) noexcept {
			for (std::size_t i = 0; i < len; ++i) {
				if (v == buf[i]) {
					RemoveAt(i);
					return;
				}
			}
		}

		// �� 0 �±��Ƴ�һ��. ֻ֧�ּ�����
		inline void RemoveFront(std::size_t const& len) {
			if constexpr (std::is_pod_v<T>) {
				assert(len <= this->len);
				if (!len) return;
				this->len -= len;
				if (this->len) {
					memmove(buf, buf + len, this->len * sizeof(T));
				}
			}
		}

		void RemoveAt(std::size_t const& idx) noexcept {
			assert(idx < len);
			--len;
			if constexpr (IsTrivial_v<T>) {
				buf[idx].~T();
				::memmove(buf + idx, buf + idx + 1, (len - idx) * sizeof(T));
			}
			else {
				for (std::size_t i = idx; i < len; ++i) {
					buf[i] = (T&&)buf[i + 1];
				}
				if constexpr (!std::is_pod_v<T>) {
					buf[len].~T();
				}
			}
		}

		// �����һ��Ԫ��������ɾ��. 
		// ͨ������Ϊ������ʻ� ��ѭ��ɨ�� if (list.len) { for (auto i = list.len - 1; i != (std::size_t)-1; --i) { ...
		// ͨ����һ��Ϊ list[list.len - 1]->idx = o->idx;
		void SwapRemoveAt(std::size_t const& idx) noexcept {
			if (idx + 1 < len) {
				std::swap(buf[idx], buf[len - 1]);
			}
			len--;
			if constexpr (!std::is_pod_v<T>) {
				buf[len].~T();
			}
		}


		// �� T ��һ��������캯���Ĳ�����׷�ӹ���һ�� item
		template<typename...Args>
		T& Emplace(Args&&...args) noexcept {
			Reserve(len + 1);
			return *new (&buf[len++]) T(std::forward<Args>(args)...);
		}

		// �ò���ֱ�ӹ���һ�� item ��ָ��λ��
		template<typename...Args>
		T& EmplaceAt(std::size_t const& idx, Args&&...args) noexcept {
			Reserve(len + 1);
			if (idx < len) {
				if constexpr (IsTrivial_v<T>) {
					::memmove(buf + idx + 1, buf + idx, (len - idx) * sizeof(T));
				}
				else {
					new (buf + len) T((T&&)buf[len - 1]);
					for (std::size_t i = len - 1; i > idx; --i) {
						buf[i] = (T&&)buf[i - 1];
					}
					if constexpr (!std::is_pod_v<T>) {
						buf[idx].~T();
					}
				}
			}
			else idx = len;
			++len;
			new (buf + idx) T(std::forward<Args>(args)...);
			return buf[idx];
		}

		// �� Emplace ��ͬ����, �����֧��1�������Ĺ��캯��, ��һ��׷�Ӷ��
		template<typename ...TS>
		void Add(TS&&...vs) noexcept {
			std::initializer_list<int> n{ (Emplace(std::forward<TS>(vs)), 0)... };
			(void)(n);
		}

		void AddRange(T const* const& items, std::size_t const& count) noexcept {
			Reserve(len + count);
			if constexpr (IsTrivial_v<T>) {
				::memcpy(buf + len, items, count * sizeof(T));
			}
			else {
				for (std::size_t i = 0; i < count; ++i) {
					new (&buf[len + i]) T((T&&)items[i]);
				}
			}
			len += count;
		}

		template<typename T2>
		void AddRange(List<T2> const& list) noexcept {
			return AddRange(list.buf, list.len);
		}

		// ����ҵ��ͷ�������. �Ҳ��������� std::size_t(-1)
		std::size_t Find(T const& v) const noexcept {
			for (std::size_t i = 0; i < len; ++i) {
				if (v == buf[i]) return i;
			}
			return std::size_t(-1);
		}

		// ������ڷ��������ľͷ��� true
		bool Exists(std::function<bool(T const& v)>&& cond) const noexcept {
			if (!cond) return false;
			for (std::size_t i = 0; i < len; ++i) {
				if (cond(buf[i])) return true;
			}
			return false;
		}

		// ֧�� for( auto&& c : list ) �﷨.
		struct Iter
		{
			T* ptr;
			bool operator!=(Iter const& other) noexcept { return ptr != other.ptr; }
			Iter& operator++() noexcept { ++ptr; return *this; }
			T& operator*() noexcept { return *ptr; }
		};
		Iter begin() noexcept { return Iter{ buf }; }
		Iter end() noexcept { return Iter{ buf + len }; }
		Iter begin() const noexcept { return Iter{ buf }; }
		Iter end() const noexcept { return Iter{ buf + len }; }


		// Object �ӿ�֧��
		virtual uint16_t GetTypeId() const noexcept override {
			return TypeId_v<List<T>>;
		}
		void ToBBuffer(BBuffer& bb) const noexcept override;
		int FromBBuffer(BBuffer& bb) noexcept override;

		int InitCascade(void* const& o) noexcept override {
			if constexpr (std::is_base_of_v<Object, T>) {
				for (std::size_t i = 0; i < len; ++i) {
					if (int r = buf[i].InitCascade(o)) return r;
				}
			}
			else if constexpr (xx::IsShared_v<T>) {
				if constexpr (std::is_base_of_v<Object, typename T::element_type>) {
					for (std::size_t i = 0; i < len; ++i) {
						if (buf[i]) {
							if (int r = buf[i]->InitCascade(o)) return r;
						}
					}
				}
			}
			return 0;
		}

		void ToString(std::string& s) const noexcept override {
			if (toStringFlag) {
				Append(s, "{ ... }");
				return;
			}
			SetToStringFlag();
			Append(s, "[ ");
			for (std::size_t i = 0; i < len; i++) {
				Append(s, buf[i], ", ");
			}
			if (len) {
				s.resize(s.size() - 2);
				Append(s, " ]");
			}
			else {
				s[s.size() - 1] = ']';
			}
			SetToStringFlag(false);
		}
	};

	// ��ʶ�ڴ���ƶ�
	template<typename T, std::size_t reservedHeaderLen>
	struct IsTrivial<List<T, reservedHeaderLen>, void> {
		static const bool value = true;
	};

	template<typename T, std::size_t reservedHeaderLen = 0>
	using List_s = std::shared_ptr<List<T, reservedHeaderLen>>;

	template<typename T, std::size_t reservedHeaderLen = 0>
	using List_w = std::shared_ptr<List<T, reservedHeaderLen>>;
}
