#include "Joypad.h"
#include "MainBus.h"
#include "SFML/Window.hpp"

namespace P4Boy
{

	class AddressAction_Joypad : public AddressAction
	{
	public:
		AddressAction_Joypad(uint8_t& value) : _value(value) {}
		virtual uint8_t Get(Address addr) const { return _value; }
		virtual void Set(Address addr, uint8_t value)
		{
			value &= 0b00110000;
			_value = value | (_value & ~0b00110000);
		}
	private:
		uint8_t& _value;
	};


	Joypad::Joypad()
	{
		_joypadThread = std::thread(&Joypad::UpdateState, this);
		_joypadState = &_joypadStateDirection;
	}

	void Joypad::ConnectAddressRange(MainBus& mainBus)
	{
		mainBus.AddSingle(0xFF00, new AddressAction_SingleAction(
			[this](Address addr, uint8_t value)
			{
				if (value & ~0b00100000)
				{
					this->_joypadState = &this->_joypadStateAction;
				}
				else
				{
					this->_joypadState = &this->_joypadStateDirection;
				}
			},
			[this](Address addr) -> uint8_t
			{
				return *this->_joypadState;
			}));
	}


	void Joypad::UpdateState()
	{
		while (true)
		{
			uint8_t joypadStateDirection = 0b00010000;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				joypadStateDirection |= 0x1 << 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				joypadStateDirection |= 0x1 << 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				joypadStateDirection |= 0x1 << 2;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				joypadStateDirection |= 0x1 << 3;
			}
			joypadStateDirection = ~joypadStateDirection & 0b00111111;


			uint8_t joypadStateAction = 0b00100000;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				joypadStateAction |= 0x1 << 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				joypadStateAction |= 0x1 << 1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			{
				joypadStateAction |= 0x1 << 2;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			{
				joypadStateAction |= 0x1 << 3;
			}
			joypadStateAction = ~joypadStateAction & 0b00111111;

			_joypadStateAction = joypadStateAction;
			_joypadStateDirection = joypadStateDirection;
		}
	}

	void Joypad::Tick()
	{
		
	}
}