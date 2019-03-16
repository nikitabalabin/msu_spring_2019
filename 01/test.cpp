#include "numbers.dat"
#include <iostream>

bool isprime(int n);

bool isprime(int n)
{
	
	// Пользуемся тем, что простые числа, 
	// которые больше 3, имеют вид 6k-1 или 6k+1.
	
	if (n == 2)
		return true;
	if (n == 3)
		return true;
	if (n % 2 == 0)
		return false;
	if (n % 3 == 0)
		return false;

	int i = 5;
	int d = 2;

	while (i * i <= n) 
	{
		if (n % i == 0)
			return false;

		i += d;
		d = 6 - d;
	}

	if (n == 1)
		return false;

	return true;
}

int main(int argc, char* argv[])
{
	// Проверка корректности ввода.
	if ((argc % 2 == 0) || (argc ==1))
		return -1;

	int buf = 0;
	int cnt = 0;
	int check = 0;
	int start = 0;
	int end = 0;

	for (int i = 1; i < argc; i += 2)
	{
		start = std::atoi(argv[i]);
		end = std::atoi(argv[i+1]);

		cnt = 0;
		check = 0;

		if (end <= Data[Size-1])	
			for (int j = 0; j < Size; j++)
			{
				buf = Data[j];

				if ((buf >= start) && (buf <= end))
				{
					if (buf == start)
						check++;

					if (buf == end)
						check++;

					if (isprime(buf))
						cnt++;
				}

				// Прервать, если start не входит в Data.
				if ((buf > start) && (check < 1))
					break;

				if (buf > end)
					break;

			}
		else
			check=0;

		if (check >= 2)
			std::cout << cnt << std::endl;
		else
			std::cout << 0 << std::endl;

	}

	return 0;
}
