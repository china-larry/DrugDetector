#include "base.h"
#include "IObjectBase.h"

#ifndef IOBJECTLIST_H__
#define IOBJECTLIST_H__


/**	对象列表，用于存放对象容器，线性容器
放入列表中的对象会增加引用计数，
当销毁IList会减少列表中对象的引用计数
*/
struct IObjectList : public IObjectBase
{
	/**
	\brief 获取对象的个数
	*/
	virtual UINT GetCount() = 0;

	/**
	\brief 根据下标获取对象，如果下标不合理，则会返回NULL，返回的对象需要调用Release()
	*/
	virtual IObjectBase* Get(UINT index) = 0;

	/**
	\brief 添加对象到容器中
	*/
	virtual NRET Add(IObjectBase* obj) = 0;

	/**
	\brief 删除掉容器中的对象
	*/
	virtual NRET Delete(UINT index) = 0;

	/**
		\brief 清空对象列表，会自动调用每个元素的Release
	*/
	virtual void Clear() = 0;

};

#ifdef WIN32
HL7_ENGINE_DLL_API IObjectList* CreateObjectList();
HL7_ENGINE_DLL_API void CopyObjectList(IObjectList* src, IObjectList* dest);
#else
IObjectList* CreateObjectList();
void CopyObjectList(IObjectList* src, IObjectList* dest);

#endif


#endif
