#pragma once

#include "Utility.h"

namespace P4Boy
{
	template<typename _registerType, typename _valueType>
	class RegisterOperation
	{
		public:
			typedef _valueType valueType;
			inline operator valueType() { return Get(); }
			valueType Get() { return reinterpret_cast<_registerType*>(this)->Get(); }
	};

	template<typename numberType>
	class Register_generic : public RegisterOperation<Register_generic<numberType>, numberType>
	{
	public:
		inline Register_generic() : _value(0) {}

		inline Register_generic& operator=(numberType value) { _value = value; return *this; }
		numberType Get() { return _value; };

	protected:
		numberType _value;
	};

	typedef Register_generic<uint8_t> Register_8b;
	typedef Register_generic<uint8_t> Register_1b;

	class Register_16b : public RegisterOperation<Register_16b, uint16_t>
	{
		public:
			inline Register_16b() : _value(0) {}

			inline Register_16b& operator=(uint16_t value) { _value = value; return *this; }
		
			inline uint8_t High() const { return _value >> 8; }
			inline uint8_t Low() const { return _value & 0xFF; }

			uint16_t Get() { return _value; };

		protected:
			uint16_t _value;
	};

	class SubRegister_8b_Low : public RegisterOperation<SubRegister_8b_Low, uint8_t>
	{
	public:
		SubRegister_8b_Low(Register_16b& parent) : _register(parent) {}
		inline SubRegister_8b_Low& operator=(uint16_t value) { _register = (_register & 0xFF00) | value; return *this; };
		uint8_t Get() { return _register.Low(); };
	private:
		Register_16b& _register;
	};

	class SubRegister_8b_High : public RegisterOperation<SubRegister_8b_High, uint8_t>
	{
		public:
			SubRegister_8b_High(Register_16b& parent) : _register(parent) {}
			inline SubRegister_8b_High& operator=(uint16_t value) { _register = (_register & 0x00FF) | (value << 8); return *this; }
			uint8_t Get() { return _register.High(); }
		private:
			Register_16b& _register;
	};

	template<typename parent_register>
	class SubRegister_1b : public RegisterOperation<SubRegister_1b<parent_register>, bool>
	{
		typedef typename parent_register::valueType parent_vtype;
		public:
			SubRegister_1b(parent_register& parent, char bitShift) : _register(parent), _bitShift(bitShift) {}
			inline SubRegister_1b& operator=(parent_vtype value) { value <<= _bitShift; _register = (_register & ~parent_vtype(1 << _bitShift)) | value; return *this; }
			bool Get() { return (_register.Get() >> _bitShift) & 0x1; }
		private:
			parent_register& _register;
			char _bitShift;
	};

	template<typename parent_register>
	class SubRegister_2b : public RegisterOperation<SubRegister_2b<parent_register>, uint8_t>
	{
		typedef typename parent_register::valueType parent_vtype;
	public:
		SubRegister_2b(parent_register& parent, char bitShift) : _register(parent), _bitShift(bitShift), _mask(~parent_vtype(0b11 << _bitShift)) {}
		inline SubRegister_2b& operator=(parent_vtype value) { value <<= _bitShift; _register = (_register & _mask) | value; return *this; }
		uint8_t Get() { return (_register.Get() >> _bitShift) & 0b11; }
	private:
		parent_register& _register;
		char _bitShift;
		parent_vtype _mask;
	};

	
}

