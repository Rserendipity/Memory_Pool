#pragma once
#include <iostream>

namespace cjj_memory_pool {
	// �ڴ����Ŀ����Ҫ���տռ䣬ֱ������������ͷſռ�
	// ������ר���ڴ��
	template<class T>
	class FixedMemoryPool {
	public:
		// ���벻���ռ�ʱ�����׳�bad_alloc�쳣
		T* New() throw(std::bad_alloc())
		{
			T* obj = nullptr;

			// ���ѹ黹�Ŀռ䣬ʹ�ù黹�Ŀռ���з���
			if(_freeList) 
			{
				// ͷɾ��
				obj = reinterpret_cast<T*>(_freeList);         // ʹ��C++11��������ת��
				_freeList = *reinterpret_cast<void**>(_freeList);
			}
			else // û�й黹�Ŀռ�
			{
				if (_leftMemSize < sizeof(T)) // �ڴ治�㣬��������һ��ռ�
				{
					_leftMemSize = 128 * 1024;
					_memory = reinterpret_cast<char*>(malloc(_leftMemSize));
					if (_memory == nullptr) // δ���뵽�㹻�ռ䣬�׳�bad_alloc�쳣
						throw std::bad_alloc();
				}
				// ���ٷ���4/8�ֽڿռ䣬�����㹻�Ŀռ���黹����
				size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
				obj = reinterpret_cast<T*>(_memory);
				_memory += objSize;
				_leftMemSize -= objSize;
			}

			// ��ʾ����T���͵Ĺ��캯�� -- ��λnew
			new(obj)T;

			return obj;
		}
		
		void Delete(T* obj) noexcept // �˺������������쳣
		{
			// ����
			obj->~T();
			
			// ʹ��ͷ�巨���ѹ黹�Ŀռ������
			// ת����void**�������ã��Ϳ��Եõ�ͷ����4/8�ֽڣ���������ڵ㣬����ͷ�巨
			*reinterpret_cast<void**>(obj) = _freeList;
			_freeList = obj;
		}


	private:
		// ���ٵĴ���ڴ�
		char* _memory = nullptr;

		// �黹�ռ����������
		void* _freeList = nullptr;

		// ʣ��ռ�Ĵ�С
		size_t _leftMemSize = 0;
	};

};