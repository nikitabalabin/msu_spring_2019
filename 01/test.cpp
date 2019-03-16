#include "numbers.dat"
#include <iostream>

int findPosition(const int arr[], int l, int r, int x, bool position);
bool isPrime(int n);

int findPosition(const int arr[], int l, int r, int x, bool position)
{
	// position == true -- findFirst
	// postitin == false -- findLast
	
	int mid = 0;
	int midValue = 0;
	int found = -1;

	while (l <= r)
	{
		mid = l + (r - l) / 2;
		midValue = arr[mid];
	
		if (midValue < x)
			l = mid + 1;
		else if (midValue > x)
			r = mid - 1;
		else 
		{
			found = mid;
			if (position)
				r = mid - 1;
			else
				l = mid + 1;
		}
	}

	return found; 
}


bool isPrime(int n)
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

	int value = 0;
	int cnt = 0;

	int i_start = 0;
	int i_end = 0;

	for (int i = 1; i < argc; i += 2)
	{
		i_start = findPosition(Data, 0, Size, std::atoi(argv[i]), true);
		i_end = findPosition(Data, 0, Size, std::atoi(argv[i+1]), false);

		cnt = 0;

		if ((i_start != -1) && (i_end != -1))
			for (int j = i_start; j <= i_end; j++)
				if (isPrime(Data[j]))
						cnt++;

		std::cout << cnt << std::endl;

	}

	return 0;
}
