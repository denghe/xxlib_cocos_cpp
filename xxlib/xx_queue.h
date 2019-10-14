﻿#pragma once
#include "xx_object.h"

namespace xx {
	// ring buffer FIFO 队列, 能随机检索...能批量 pop. 简单测试比 std::deque 快 70% 左右
	//...............FR...............					// Head == Tail
	//......Head+++++++++++Tail.......					// DataLen = Tail - Head
	//++++++Tail...........Head+++++++					// DataLen = BufLen - Head + Tail
	template <class T>
	struct Queue {
		typedef T ChildType;
		T*				buf;
		std::size_t		cap;
		std::size_t		head = 0, tail = 0;					// FR..............................

		// 因为该队列似乎没办法令 buf 为空, 故移动操作也会创建 buf
		explicit Queue(std::size_t capacity = 8) noexcept;
		Queue(Queue && o) noexcept;
		~Queue() noexcept;

		Queue(Queue const& o) = delete;
		Queue& operator=(Queue const& o) = delete;

		T const& operator[](std::size_t const& idx) const noexcept;	// [0] = [ head ]
		T& operator[](std::size_t const& idx) noexcept;
		T const& At(std::size_t const& idx) const noexcept;			// []
		T& At(std::size_t const& idx) noexcept;

		std::size_t Count() const noexcept;
		bool Empty() const noexcept;
		void Clear() noexcept;
		void Reserve(std::size_t const& capacity, bool const& afterPush = false) noexcept;

		template<typename...Args>
		T& Emplace(Args&&...ps) noexcept;						// [ tail++ ] = T( ps )

		template<typename ...TS>
		void Push(TS&& ...vs) noexcept;

		bool TryPop(T& outVal) noexcept;

		T const& Top() const noexcept;							// [ head ]
		T& Top() noexcept;
		void Pop() noexcept;									// ++head
		std::size_t PopMulti(std::size_t const& count) noexcept;			// head += count

		T const& Last() const noexcept;							// [ tail-1 ]
		T& Last() noexcept;
		void PopLast() noexcept;								// --tail
	};

	template<typename T>
	using Queue_s = std::shared_ptr<Queue<T>>;

	template<typename T>
	using Queue_w = std::weak_ptr<Queue<T>>;
}

// impls
namespace xx
{
	template <class T>
	Queue<T>::Queue(std::size_t capacity) noexcept {
		if (capacity < 8) {
			capacity = 8;
		}
		auto bufByteLen = Round2n(capacity * sizeof(T));
		buf = (T*)malloc((std::size_t)bufByteLen);
		assert(buf);
		cap = std::size_t(bufByteLen / sizeof(T));
	}

	template <class T>
	Queue<T>::Queue(Queue&& o) noexcept
		: Queue() {
		std::swap(buf, o.buf);
		std::swap(cap, o.cap);
		std::swap(head, o.head);
		std::swap(tail, o.tail);
	}

	template <class T>
	Queue<T>::~Queue() noexcept {
		assert(buf);
		Clear();
		free(buf);
		buf = nullptr;
	}

	template <class T>
	std::size_t Queue<T>::Count() const noexcept {
		//......Head+++++++++++Tail.......
		//...............FR...............
		if (head <= tail) return tail - head;
		// ++++++Tail...........Head++++++
		else return tail + (cap - head);
	}

	template <class T>
	bool Queue<T>::Empty() const noexcept {
		return head == tail;
	}

	template <class T>
	void Queue<T>::Pop() noexcept {
		assert(head != tail);
		buf[head++].~T();
		if (head == cap) {
			head = 0;
		}
	}

	template <class T>
	void Queue<T>::PopLast() noexcept {
		assert(head != tail);
		buf[tail--].~T();
		if (tail == (std::size_t)-1) {
			tail = cap - 1;
		}
	}

	template <class T>
	template<typename...Args>
	T& Queue<T>::Emplace(Args&& ...args) noexcept {
		auto idx = tail;
		new (buf + tail++) T(std::forward<Args>(args)...);
		if (tail == cap) {			// cycle
			tail = 0;
		}
		if (tail == head) {			// no more space
			idx = cap - 1;
			Reserve(cap * 2, true);
		}
		return buf[idx];
	}

	template<typename T>
	template<typename ...TS>
	void Queue<T>::Push(TS&& ...vs) noexcept {
		std::initializer_list<int> n{ (Emplace(std::forward<TS>(vs)), 0)... };
		(void)n;
	}

	template <class T>
	void Queue<T>::Clear() noexcept {
		//........HT......................
		if (head == tail) return;

		if constexpr (!std::is_pod_v<T>) {
			//......Head+++++++++++Tail......
			if (head < tail) {
				for (auto i = head; i < tail; ++i) {
					buf[i].~T();
				}
			}
			// ++++++Tail...........Head++++++
			else {
				for (std::size_t i = 0; i < tail; ++i) {
					buf[i].~T();
				}
				for (auto i = head; i < cap; ++i) {
					buf[i].~T();
				}
			}
		}

		//........HT......................
		head = tail = 0;
	}

	template <typename T>
	std::size_t Queue<T>::PopMulti(std::size_t const& count) noexcept {
		if (count <= 0) return 0;

		auto dataLen = Count();
		if (count >= dataLen) {
			Clear();
			return dataLen;
		}
		// count < dataLen

		//......Head+++++++++++Tail......
		if (head < tail) {
			if constexpr (!std::is_pod_v<T>) {
				//......Head+++++++++++count......
				for (auto i = head; i < head + count; ++i) buf[i].~T();
			}
			head += count;
		}
		// ++++++Tail...........Head++++++
		else {
			auto frontDataLen = cap - head;
			//...Head+++
			if (count < frontDataLen) {
				if constexpr (!std::is_pod_v<T>) {
					for (auto i = head; i < head + count; ++i) buf[i].~T();
				}
				head += count;
			}
			else {
				if constexpr (!std::is_pod_v<T>) {
					//...Head++++++
					for (auto i = head; i < cap; ++i) buf[i].~T();
				}

				// <-Head
				head = count - frontDataLen;

				if constexpr (!std::is_pod_v<T>) {
					// ++++++Head...
					for (std::size_t i = 0; i < head; ++i) buf[i].~T();
				}
			}
		}
		return count;
	}

	template <class T>
	void Queue<T>::Reserve(std::size_t const& capacity, bool const& afterPush) noexcept {
		assert(capacity > 0);
		if (capacity <= cap) return;

		auto newBufByteLen = Round2n(capacity * sizeof(T));
		auto newBuf = (T*)malloc((std::size_t)newBufByteLen);
		assert(newBuf);
		auto newBufLen = std::size_t(newBufByteLen / sizeof(T));

		// afterPush: ++++++++++++++TH++++++++++++++++
		auto dataLen = afterPush ? cap : Count();

		//......Head+++++++++++Tail.......
		if (head < tail) {
			if constexpr (IsTrivial_v<T>) {
				memcpy((void*)newBuf, buf + head, dataLen * sizeof(T));
			}
			else {
				for (std::size_t i = 0; i < dataLen; ++i) {
					new (newBuf + i) T((T&&)buf[head + i]);
					buf[head + i].~T();
				}
			}
		}
		// ++++++Tail...........Head+++++++
		// ++++++++++++++TH++++++++++++++++
		else
		{
			//...Head++++++
			auto frontDataLen = cap - head;
			if constexpr (IsTrivial_v<T>) {
				memcpy((void*)newBuf, buf + head, frontDataLen * sizeof(T));
			}
			else {
				for (std::size_t i = 0; i < frontDataLen; ++i) {
					new (newBuf + i) T((T&&)buf[head + i]);
					buf[head + i].~T();
				}
			}

			// ++++++Tail...
			if constexpr (IsTrivial_v<T>) {
				memcpy((void*)(newBuf + frontDataLen), buf, tail * sizeof(T));
			}
			else {
				for (std::size_t i = 0; i < tail; ++i) {
					new (newBuf + frontDataLen + i) T((T&&)buf[i]);
					buf[i].~T();
				}
			}
		}

		// Head+++++++++++Tail.............
		head = 0;
		tail = dataLen;

		free(buf);
		buf = newBuf;
		cap = newBufLen;
	}


	template<typename T>
	T const& Queue<T>::operator[](std::size_t const& idx) const noexcept {
		return At(idx);
	}

	template<typename T>
	T& Queue<T>::operator[](std::size_t const& idx) noexcept {
		return At(idx);
	}

	template<typename T>
	T const& Queue<T>::At(std::size_t const& idx) const noexcept {
		return const_cast<Queue<T>*>(this)->At(idx);
	}

	template<typename T>
	T const& Queue<T>::Top() const noexcept {
		assert(head != tail);
		return buf[head];
	}
	template<typename T>
	T& Queue<T>::Top() noexcept {
		assert(head != tail);
		return buf[head];
	}


	template<typename T>
	T& Queue<T>::At(std::size_t const& idx) noexcept {
		assert(idx < Count());
		if (head + idx < cap) {
			return buf[head + idx];
		}
		else {
			return buf[head + idx - cap];
		}
	}


	template<typename T>
	T const& Queue<T>::Last() const noexcept {
		assert(head != tail);
		return buf[tail - 1 == (std::size_t)-1 ? cap - 1 : tail - 1];
	}
	template<typename T>
	T& Queue<T>::Last() noexcept {
		assert(head != tail);
		return buf[tail - 1 == (std::size_t)-1 ? cap - 1 : tail - 1];
	}


	template <typename T>
	bool Queue<T>::TryPop(T& outVal) noexcept {
		if (head == tail) return false;
		outVal = std::move(buf[head]);
		Pop();
		return true;
	}

}
