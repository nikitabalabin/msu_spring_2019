class proxyClass
	{
	int* row;
	size_t length;

	public:

		proxyClass(int* start, size_t len):
			length(len), row(start) {}

		~proxyClass() {}

		int& operator[] (size_t x)
			{
			if (x >= length)
				throw std::out_of_range("Index Out Of Range");
			return row[x];
			}

		const int& operator[] (size_t x) const
		{
			if (x >= length)
				throw std::out_of_range("Index Out Of Range");
			return row[x];
		}
	};

class Matrix
{
	size_t rows, columns;
	int **data = NULL;

public:
	Matrix(const size_t rows, const size_t columns):
		rows(rows), columns(columns)
		{
			data = new int* [rows];

			for (size_t i = 0; i < rows; i++)
				data[i] = new int[columns];
		}

	~Matrix()
	{
		for (size_t i = 0; i < rows; i++)
			delete[] data[i];
		delete[] data;
	}

	size_t getRows() const
	{
	    return rows;
	}

	size_t getColumns() const
	{
		return columns;
	}

	proxyClass operator[](const size_t i)
	{
		if (i >= rows)
			throw std::out_of_range("Index Out Of Range");

		proxyClass buf(data[i], columns);
		return buf;
	}

	const proxyClass operator[](const size_t i) const
	{
		if (i >= rows)
			throw std::out_of_range("Index Out Of Range");

		proxyClass buf(data[i], columns);
		return buf;
	}

	Matrix& operator*=(const size_t n)
	{
		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				data[i][j] *= n;

		return *this;
	}

	bool operator==(const Matrix& other) const
	{
		if ((other.getColumns() != columns) || (other.getRows() != rows))
			return false;

		for (size_t i = 0; i < rows; i++)
			for (size_t j = 0; j < columns; j++)
				if (data[i][j] != other[i][j])
					return false;
		return true;
	}

	bool operator!=(const Matrix& other) const
	{
		return !(*this == other);
	}
};