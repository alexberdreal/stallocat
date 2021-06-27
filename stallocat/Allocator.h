#ifndef ALLOCATOR
#define ALLOCATOR
#include <assert.h>
namespace stalloc {
	class StackAllocator {
	public:
		void allocate(size_t size) noexcept(false) {
			if (!btm) {
				btm = new char[size];
				end = size + btm;
				cur = btm;
			}
		}
		template <class T>
		void destroyDownTo(T* ptr) noexcept(false) {
			Elem* cur = elemTop;
			while (cur) {
				if (cur->ptr == ptr) break;
				cur = cur->next;
			}
			if (!cur) return;
			while (ptr) {
				if (elemTop->ptr == ptr) ptr = nullptr;
				elemTop->dtor(elemTop->ptr);
				Elem* temp = elemTop->next;
				delete elemTop;
				elemTop = temp;
			}
		}
		void dealloc() noexcept(false) {
			destroyDownTo(btm);
			delete[] btm;
		}
		protected:
		StackAllocator() = default;
		~StackAllocator() noexcept(false) {
			dealloc();
		}

		template <class T, class... Args>
		void push_back(Args&&... arg) {
			Elem* elem = new Elem;
			elem->next = elemTop;
			elem->ptr = new(cur) T(std::forward<Args>(arg)...);      // arg lvalue, passes by lvalue reference => std::forward converts it into unwrapped state (lv or rv ref)
			auto dtor = [](void* p) { ((T*)p)->~T(); };		 //elem->dtor = reinterpret_cast<T*>(elem->ptr)->~T;
			elem->dtor = dtor;
			elem->size = sizeof(T);
			cur += sizeof(T);
			elemTop = elem;
		}

		struct Elem
		{
			void* ptr;    //pointer to the object in HEAP 
			size_t size;  //size of the element
			Elem* next;	  //next elem in stack
			void(*dtor)(void*);  //destructor of an object
		};

		Elem* elemTop = nullptr;
		char* end = nullptr;
		char* btm = nullptr;
		char* cur = nullptr;
	};

	class StackAllocatorRealloc : public StackAllocator {
	public:
		StackAllocatorRealloc() = default;
		template <class T, class... Args>						// regular type
		T** construct(Args&&... arg) {					// accepts rvalue and lvalue (type unwrapping, T&& & --> T&, T&& && --> T&&)
			if ((end - cur < sizeof(T)) && elemTop) handleOverflow();
			push_back<T>(arg...);
			return reinterpret_cast<T**>(&elemTop->ptr);
		}
	private:
		void handleOverflow() noexcept(false) {
			char* nwPtr = new char[(end - btm) * 2];
			end = nwPtr + (end - btm) * 2;
			memcpy(nwPtr, btm, cur - btm);
			cur = nwPtr + (cur - btm);
			char* tptr = cur;
			Elem* tElem = elemTop;
			while (tElem) {
				tptr -= tElem->size;
				tElem->ptr = tptr;
				tElem = tElem->next;
			}
			delete[] btm;
			btm = nwPtr;
		}
	};

	class StackAllocatorNoRealloc : public StackAllocator {
	public:
		StackAllocatorNoRealloc() = default;
		template <class T, class... Args>						// regular type
		T* construct(Args&&... arg) {					// accepts rvalue and lvalue (type unwrapping, T&& & --> T&, T&& && --> T&&)
			if ((end - cur < sizeof(T)) && elemTop) return nullptr;
			push_back<T>(arg...);
			return reinterpret_cast<T*>(elemTop->ptr);
		}
	};
};



#endif // !ALLOCATOR
