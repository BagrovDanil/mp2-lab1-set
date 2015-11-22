// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int _BitLen)
{
	if (_BitLen > 0)
	{
		BitLen = _BitLen;
		MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	}
	else throw std::exception("_Bitlen <0");
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	int n1 = n % (sizeof(TELEM) * 8);
	TELEM maska = 1 << n1;
	return maska;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n <= BitLen)
	{
		int x = n;
		pMem[GetMemIndex(x)] = pMem[GetMemIndex(x)] | GetMemMask(x);
	}
	else throw std::exception("index <0 or too large");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n <= BitLen)
	{
		int x = n;
		pMem[GetMemIndex(x)] &= ~(GetMemMask(x));
	}
	else throw std::exception("index <0 or too large");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n <= BitLen)
	{
		int temp = pMem[GetMemIndex(n)];
		temp &= GetMemMask(n);
		if (temp != 0)
			return 1;
		else
			return 0;
	}
	else throw std::exception("index <0 or too large");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i<MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int s = 0;
	if (MemLen != bf.MemLen)
		return 0;
	else
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
				s = 0;
			else
			{
				s = 1;
				break;
			}
		}
	}
	if (s == 0)
		return 1;
	else
		return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int s = 0;
	if (MemLen != bf.MemLen)
		return 1;
	else
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
				s = 1;
			else
			{
				s = 0;
				break;
			}
		}
	}
	if (s == 0)
		return 1;
	else
		return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int tmpBL;
	int minMem;
	int n = 0;
	if (BitLen >= bf.BitLen)
		tmpBL = BitLen;
	else
		tmpBL = bf.BitLen;
	if (MemLen >= bf.MemLen)
		minMem = bf.MemLen;
	else minMem = MemLen;
	TBitField tmp(tmpBL);
	while (n<minMem)
	{
		tmp.pMem[n] = pMem[n] | bf.pMem[n];
		n++;
	}
	if (MemLen>bf.MemLen)
		for (int i = minMem; i < MemLen; i++)
			tmp.pMem[i] = pMem[i];
	else
		for (int i = minMem; i < bf.MemLen; i++)
			tmp.pMem[i] = bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int tmpBL;
	int minMem;
	int n = 0;
	if (BitLen >= bf.BitLen)
		tmpBL = BitLen;
	else
		tmpBL = bf.BitLen;
	if (MemLen >= bf.MemLen)
		minMem = bf.MemLen;
	else minMem = MemLen;
	TBitField tmp(tmpBL);
	while (n<minMem)
	{
		tmp.pMem[n] = pMem[n] & bf.pMem[n];
		n++;
	}
	if (MemLen>bf.MemLen)
		for (int i = minMem; i < MemLen; i++)
			tmp.pMem[i] = 0;
	else
		for (int i = minMem; i < bf.MemLen; i++)
			tmp.pMem[i] = 0;
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (int i = 0; i < BitLen; i++)
	{
		if ((tmp).GetBit(i) == 1){
			tmp.ClrBit(i);
		}
		else
			tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	while ((i >= 0) && (i < bf.BitLen))
	{
		bf.SetBit(i);
		istr >> i;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
