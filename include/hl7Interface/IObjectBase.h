#ifndef IOBJECTBASE_H__
#define IOBJECTBASE_H__

#include "base.h"
#include <string>
/**
\brief 对象类型,分为基础类型和子类型

\see type_t

*/
class ObjectType
{
	type_t baseType;
	type_t subType;

public:
	type_t GetBaseType()const { return baseType; }
	type_t GetSubType()const { return subType; }



	ObjectType(type_t basicType, type_t subType)
	{
		this->baseType = basicType;
		this->subType = subType;
	}

	ObjectType(const ObjectType& objType)
	{
		baseType = objType.baseType;
		subType = objType.subType;
	}

	ObjectType& operator= (const ObjectType& objType)
	{
		baseType = objType.baseType;
		subType = objType.subType;

		return *this;
	}

	ObjectType()
	{
		baseType = TypeObjectBase;
		subType = SubTypeInvaild;
	}
};

template<type_t basicType, type_t subType>
class ObjectTypeT
{
public:
	ObjectTypeT() :
		objType(basicType, subType)
	{

	}

	operator ObjectType() const
	{
		return objType;
	}
private:
	ObjectType objType;
};


/**
\brief 基类接口
*/
struct IObjectBase
{
	/**
	\brief 增加对象引用计数
	*/
	virtual unsigned int AddRef() = 0;

	/**
	\brief 释放对象，当对象引用计数为0，彻底销毁对象
	*/
	virtual void Release() = 0;

	/**
	\brief 查询接口
	*/
	virtual NRET QueryIF(IFID, void**) = 0;

	/**
	\brief 获取对象类型信息
	*/
	virtual void GetType(ObjectType& ) = 0;

	/**
	\brief 获取父对象，获取到得父对象，引用计数会+1
	*/
	virtual IObjectBase* GetParent() = 0;

	/**
	\brief 设置父对象，原有的父对象会调用Release进行释放
	*/
	virtual NRET SetParent(IObjectBase* parent) = 0;
};

class EncodingCharacters
{
public:
	EncodingCharacters()
	{
		m_fieldSeparator = '|';
		m_componentSeparator = '^';
		m_repetitionSeparator = '~';
		m_escapeCharacter = '\\';
		m_subcomponentSeparator = '&';
	}
	EncodingCharacters(char fieldSeparator, std::string encodingChars)
	{
		m_fieldSeparator = fieldSeparator;
		if (encodingChars.length()<4)
		{
			m_componentSeparator = '^';
			m_repetitionSeparator = '~';
			m_escapeCharacter = '\\';
			m_subcomponentSeparator = '&';
		}
		else
		{
			m_componentSeparator = encodingChars.at(0);
			m_repetitionSeparator = encodingChars.at(1);
			m_escapeCharacter = encodingChars.at(2);
			m_subcomponentSeparator= encodingChars.at(3);
		}
	}
public:
	char m_fieldSeparator;
	char m_componentSeparator;
	char m_repetitionSeparator;
	char m_escapeCharacter;
	char m_subcomponentSeparator;
};
#endif