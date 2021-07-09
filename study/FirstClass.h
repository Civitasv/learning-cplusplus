#pragma once
class FirstClass
{
public:
	char s_;
	double length_;
	double get_length();
	void set_length(double length);
	virtual void get() = 0; // ´¿Ðéº¯Êý
};