#pragma once
#include "2A03.hpp"

using namespace MicroSim::Ricoh;
//DONE
void Core2A03::addrm_abs()
{
	low(_addr) = read_byte(PC++);
	addrm = &Core2A03::addrm_abs2;
}
//DONE
void Core2A03::addrm_abs2()
{
	high(_addr) = read_byte(PC++);
	state = State::EXEC;
}
//DONE
void Core2A03::addrm_absx()
{
	low(_addr) = read_byte(PC++);
	addrm = &Core2A03::addrm_absx2;
}
//DONE
void Core2A03::addrm_absx2()
{
	high(_addr) = read_byte(PC++);
	low(_addr) += X;
	if(low(_addr) < X){
		op = &Core2A03::addrm_absx3;
	}else{
		state = State::EXEC;
	}
}
//DONE
void Core2A03::addrm_absx3()
{
	read_byte(_addr);
	high(_addr)++;
	state = State::EXEC;
}

//DONE
void Core2A03::addrm_absx_rmw()
{
	low(_addr) = read_byte(PC++);
	addrm = &Core2A03::addrm_absx2_rmw;
}
//DONE
void Core2A03::addrm_absx2_rmw()
{
	high(_addr) = read_byte(PC++);
	addrm = &Core2A03::addrm_absx3_rmw;
}
//DONE
void Core2A03::addrm_absx3_rmw()
{
	read_byte(high(_addr)<<8 | (low(_addr)+X));
	_addr += X;
	state = State::EXEC;
}

//DONE
void Core2A03::addrm_absy()
{
	low(_addr) = read_byte(PC++);
	addrm = &Core2A03::addrm_absy2;
}
//DONE
void Core2A03::addrm_absy2()
{
	high(_addr) = read_byte(PC++);
	low(_addr) += Y;
	if(low(_addr) < X){
		op = &Core2A03::addrm_absy3;
	}else{
		state = State::EXEC;
	}
}
//DONE
void Core2A03::addrm_absy3()
{
	read_byte(_addr);
	high(_addr)++;
	state = State::EXEC;
}

//DONE
void Core2A03::addrm_acc()
{
	state = State::EXEC;
	(this->*op)();
}

//DONE
void Core2A03::addrm_immb()
{
	_addr = PC;
	PC++;
	state = State::EXEC;
	(this->*op)();
}
//DONE
void Core2A03::addrm_immw()
{
	_addr = PC;
	PC += 2;
	state = State::EXEC;
	(this->*op)();
}

void Core2A03::addrm_imp()
{
	read_byte(PC);
	state = State::EXEC;
	(this->*op)();
}

void Core2A03::addrm_zp()
{
	_addr = read_byte(PC++);
	state = State::EXEC;
}

void Core2A03::addrm_zpxi()
{
	_addr = read_byte(PC++);
	low(_addr) += X;
	addrm = &Core2A03::addrm_zpxi2;
}
void Core2A03::addrm_zpxi2()
{
	high(_addr) = read_byte(low(_addr));
	addrm = &Core2A03::addrm_zpxi3;
}
void Core2A03::addrm_zpxi3()
{
	low(_addr) = read_byte(low(_addr)+1);
	std::swap(low(_addr), high(_addr));
	state = State::EXEC;
}

void Core2A03::addrm_zpiy()
{
	_addr = read_byte(PC++);
	addrm = &Core2A03::addrm_zpiy2;
}
void Core2A03::addrm_zpiy2()
{
	high(_addr) = read_byte(low(_addr));
	addrm = &Core2A03::addrm_zpiy3;
}
void Core2A03::addrm_zpiy3()
{
	low(_addr) = read_byte(low(_addr)+1);
	std::swap(low(_addr), high(_addr));
	low(_addr) += Y;
	if(low(_addr) >= Y)
		state = State::EXEC;
	addrm = &Core2A03::addrm_zpiy4;
}
void Core2A03::addrm_zpiy4()
{
	read_byte(_addr);
	high(_addr)++;
	state = State::EXEC;
}

//DONE
void Core2A03::addrm_zpx()
{
	_addr = read_byte(PC++);
	addrm = &Core2A03::addrm_zpx2;
}
//DONE
void Core2A03::addrm_zpx2()
{
	read_byte(_addr);
	low(_addr) += X;
	state = State::EXEC;
}
//DONE
void Core2A03::addrm_zpy()
{
	_addr = read_byte(PC++);
	addrm = &Core2A03::addrm_zpy2;
	low(_addr) += Y;
	state = State::EXEC;
}
//DONE
void Core2A03::addrm_zpy2()
{
	read_byte(_addr);
	low(_addr) += Y;
	state = State::EXEC;
}

void Core2A03::addrm_pcr()
{
	_addr = read_byte(PC++);
	state = State::EXEC;
	(this->*op)();
}

// void Core2A03::addrm_unused2(){
// 	read_byte(PC);
// 	addrm = &Core2A03::addrm_unused;
// }
void Core2A03::addrm_unused(){
	read_byte(PC);
	state = State::EXEC;
}




