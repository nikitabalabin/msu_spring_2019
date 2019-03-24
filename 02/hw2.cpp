#include <iostream> 
#include <string>


class Calculator
{
	bool isAdd(char x);
	bool isMult(char x);
	bool isOperator(char x);
	bool isDigit(char x);

	bool cleanSpace();
	bool check();
	bool shiftUnarMinus();
	bool cleanUnarMinus();
	bool doMult();
	bool doAdd();
	
	std::string sub_result = "";

public:

	int64_t result = 0;
	bool calculate(const std::string& s);
};

bool Calculator::isAdd(char x)
{
	if ((x == '-') || (x == '+'))
		return true;
	else
		return false;
}

bool Calculator::isMult(char x)
{
	if ((x == '*') || (x == '/'))
		return true;
	else
		return false;
}

bool Calculator::isOperator(char x)
{
	if (isAdd(x) || isMult(x))
		return true;
	else
		return false;
}

bool Calculator::isDigit(char x)
{
	if (('0' <= x) && (x <= '9'))
		return true;
	else
		return false;
}

bool Calculator::cleanSpace()
{
	std::string buf = "";

	for (uint64_t i = 0; i < sub_result.size(); i++)
		if (sub_result[i] != ' ')
			buf += sub_result[i];

	sub_result = buf;

	return true;
}

bool Calculator::check()
{
	if (!isDigit(sub_result[0]) && (sub_result[0] != '-'))
		return false;
	if (!isDigit(sub_result[sub_result.size() - 1]))
		return false;

	bool firstDigit = true;
	for (uint64_t i = 0; i < sub_result.size(); i++)
		if(isDigit(sub_result[i]))
		{
			firstDigit = true;
			continue;
		}
		else if(isOperator(sub_result[i]))
			if (firstDigit)
			{
				firstDigit = false;
				continue;
			}
			else if (sub_result[i] == '-')
				continue;
			else
				return false;
		else
			return false;

	return true;
}

bool Calculator::shiftUnarMinus()
{
	int64_t firstDigit = 0;
	for(uint64_t i = 0; i < sub_result.size() - 1; i++)
	{
		if (isAdd(sub_result[i]))
			firstDigit = i;
		if (isMult(sub_result[i]) && isAdd(sub_result[i+1]))
		{
			sub_result.insert(firstDigit + 1, 1, sub_result[i+1]);
			sub_result.erase(i + 2, 1);
		}
	}

	return true;
}

bool Calculator::cleanUnarMinus()
{
	std::string buf = "";
	int64_t cnt_minus = 0;

	for (uint64_t i=0; i < sub_result.size(); i++)
	{
		if (isAdd(sub_result[i]))
			while (sub_result[i+cnt_minus+1] == '-')
				cnt_minus++;

		if (cnt_minus % 2 == 0)
			buf += sub_result[i];
		else if (sub_result[i] == '-')
			buf += "+";
		else
			buf += "-";

		i += cnt_minus;
		cnt_minus = 0;
	}

	sub_result = buf;

	return true;
}




bool Calculator::doMult()
{
	uint64_t pos = 0;

	while(!isMult(sub_result[pos]))
	{
		if (pos == sub_result.size())
			return true;
		pos++;
	}
	
	int64_t leftpos = pos;

	while(!isOperator(sub_result[leftpos-1]))
	{
		leftpos--;

		if (leftpos == 0)
			break;
	}
	
	int64_t left = std::stoi(sub_result.substr(leftpos, pos - leftpos));

	uint64_t rightpos = pos;
	
	int64_t right = 0;
	int64_t mult_result = left;

	while (!isAdd(sub_result[rightpos]) && (rightpos < sub_result.size()))
	{
		pos = rightpos;
		while(!isOperator(sub_result[rightpos+1]))
		{
			rightpos++;

			if (rightpos == sub_result.size())
				break;
		}
	
		right = std::stoi(sub_result.substr(pos + 1, rightpos - pos));

		if (sub_result[pos] == '/')
			if (right == 0)
				return false;
			else
				mult_result /= right;
		else
			mult_result *= right;

		rightpos++;
	}
	
	sub_result.replace(leftpos, rightpos - leftpos, std::to_string(mult_result));

	return doMult();
}

bool Calculator::doAdd()
{
	
	uint64_t end = 1;

	while(!isAdd(sub_result[end+1]))
	{
		if (end == sub_result.size()-1)
			break;

		end++;
	}

	result += std::stoi(sub_result.substr(0, end + 1));

	if (end == sub_result.size() - 1)
		return true;

	sub_result.erase(0, end + 1);

	return doAdd();
}

bool Calculator::calculate(const std::string& s)
{
	sub_result = s;
	result = 0;

	cleanSpace();

	if (!check())
		return false;

	if (!isOperator(sub_result[0]))
		sub_result = "+" + sub_result;

	shiftUnarMinus();

	cleanUnarMinus();

	if (!doMult())
		return false;

	doAdd();

	return true;
}


int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		std::cout << "error" << std::endl;
		return 1;
	}

	std::string s;
	Calculator calc;

	s = argv[1];

	if (calc.calculate(s))
		std::cout << calc.result << std::endl;
	else
	{
		std::cout << "error" << std::endl;
		return 1;
	}

	return 0;
}