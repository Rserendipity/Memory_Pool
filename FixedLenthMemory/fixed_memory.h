#pragma once
#include <iostream>

namespace cjj_memory_pool {
	// 内存池项目不需要回收空间，直到程序结束才释放空间
	// 定长的专用内存池
	template<class T>
	class FixedMemoryPool {
	public:
		// 申请不到空间时，会抛出bad_alloc异常
		T* New() throw(std::bad_alloc())
		{
			T* obj = nullptr;

			// 有已归还的空间，使用归还的空间进行分配
			if(_freeList) 
			{
				// 头删法
				obj = reinterpret_cast<T*>(_freeList);         // 使用C++11风格的类型转换
				_freeList = *reinterpret_cast<void**>(_freeList);
			}
			else // 没有归还的空间
			{
				if (_leftMemSize < sizeof(T)) // 内存不足，重新申请一块空间
				{
					_leftMemSize = 128 * 1024;
					_memory = reinterpret_cast<char*>(malloc(_leftMemSize));
					if (_memory == nullptr) // 未申请到足够空间，抛出bad_alloc异常
						throw std::bad_alloc();
				}
				// 至少分配4/8字节空间，留出足够的空间给归还链表
				size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
				obj = reinterpret_cast<T*>(_memory);
				_memory += objSize;
				_leftMemSize -= objSize;
			}

			// 显示调用T类型的构造函数 -- 定位new
			new(obj)T;

			return obj;
		}
		
		void Delete(T* obj) noexcept // 此函数不会引发异常
		{
			// 析构
			obj->~T();
			
			// 使用头插法，把归还的空间挂起来
			// 转换成void**，解引用，就可以得到头部的4/8字节，当作链表节点，进行头插法
			*reinterpret_cast<void**>(obj) = _freeList;
			_freeList = obj;
		}


	private:
		// 开辟的大块内存
		char* _memory = nullptr;

		// 归还空间的自由链表
		void* _freeList = nullptr;

		// 剩余空间的大小
		size_t _leftMemSize = 0;
	};

};