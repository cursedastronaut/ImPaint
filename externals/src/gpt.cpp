#include <gpt.hpp>

//STR CLASS
std::string GPT::str::lowercase(std::string in)
{
	for (size_t i = 0; i < in.size(); i++)
		if (in[i] >= 0x41 && in[i] <= 0x5a)
			in[i] += 32;
	return in;
};

std::string GPT::str::uppercase(std::string in)
{
	for (size_t i = 0; i < in.size(); i++)
		if (in[i] >= 0x61 && in[i] <= 0x7a)
			in[i] -= 32;
	return in;
}

std::string GPT::str::getfirstword(std::string in)
{
	std::string out;
	out = in;
	size_t wordSize;
	for (size_t i = 0; i < in.size(); i++)
		if (out[i] == ' ' || out[i] == '\n' || out[i] == '\0')
		{
			wordSize = i;
			out[i] = '\0';
			out.resize(wordSize + 1);
			break;
		}

	return out;
}

std::string GPT::str::includes(std::string in, std::string word)
{
	std::string output;
	return output;
}

void GPT::str::split(std::string in, char splitter, std::vector<std::string>* output)
{
	size_t startIndex = 0, endIndex = 0;
	for (size_t i = 0; i <= in.size(); i++) {

		// If we reached the end of the word or the end of the input.
		if (in[i] == splitter || i == in.size()) {
			endIndex = i;
			std::string temp;
			temp.append(in, startIndex, endIndex - startIndex);
			output->push_back(temp);
			startIndex = endIndex + 1;
		}
	}
	return;
}

bool GPT::str::startsWith(std::string input, std::string start)
{
	for (size_t i = 0; i < input.size() && i < start.size(); ++i)
		if (input[i] != start[i])
			return false;
	return true;
}

bool GPT::str::endsWith(std::string input, std::string end)
{
	size_t inputSize = input.size();
	size_t endSize = end.size();

	// Check if the input is shorter than the end, it cannot end with it
	if (inputSize < endSize) {
		return false;
	}

	// Compare characters from the end of the input and end strings
	for (size_t i = 0; i < endSize; ++i) {
		if (input[inputSize - endSize + i] != end[i]) {
			return false;
		}
	}

	return true;
}



std::string GPT::str::splice(std::string input, size_t begin, size_t end, bool keeplastbyte)
{
	std::string output;
	if (end == 0)
		end = input.size();

	output.resize(end - begin + 1);
	ASSERT(begin >= input.size(), "Start value is larger than the string size.");


	for (size_t i = 0; i < end - begin && i < input.size(); i++)
		output[i] = input[i+begin];
	if (keeplastbyte)
		return output;

	output.resize(output.size()-1);
	return output;
}

//STR::STRING CLASS
GPT::str::string::string() {}
GPT::str::string::string(std::string in)				{	value = in;	}
GPT::str::string::string(const char in)					{	value = in;	}
GPT::str::string::string(const char* in)				{	value = in;	}

std::string			GPT::str::string::operator=(string in)				{	return in.value;	}
GPT::str::string	GPT::str::string::operator+(int in)					{	return std::to_string(in);	}
GPT::str::string	GPT::str::string::operator=(const char input)		{	return string(input);	}
GPT::str::string*	GPT::str::string::operator=(const char* input)		{	value = input;	return this;	}
char&				GPT::str::string::operator[](int index)				{	return value[index];	}
char				GPT::str::string::operator[](int index) const		{	return value[index];	}
bool				GPT::str::string::operator==(string in)				{	return in.value == value;	}
					GPT::str::string::operator	std::string()			{	return value;	}

size_t 				GPT::str::string::size()							{	return value.size();	}
std::string			GPT::str::string::get_value()						{	return value;	}

void				GPT::str::string::lowercase_n()						{	value = lowercase(value);	};

void				GPT::str::string::uppercase_n()						{	value = uppercase(value);	};

std::string			GPT::str::string::getfirstword_n()					{	return getfirstword(value);	}

GPT::str::string	GPT::str::string::getfirstword_nn()					{	return string(getfirstword(value));	}
bool 				GPT::str::string::startsWith_n(std::string start)	{	return startsWith(value, start);	}

void GPT::str::string::split_n(char splitter, std::vector<std::string>* output)
{	split(value, splitter, output);	}
void GPT::str::string::split_n(char splitter, std::vector<string>* output)
{
	size_t startIndex = 0, endIndex = 0;
	for (size_t i = 0; i <= value.size(); i++) {

		// If we reached the end of the word or the end of the input.
		if (value[i] == splitter || i == value.size()) {
			endIndex = i;
			std::string temp;
			temp.append(value, startIndex, endIndex - startIndex);
			output->push_back(string(temp));
			startIndex = endIndex + 1;
		}
	}
	return;
}

std::string GPT::str::string::splice_s(size_t begin, size_t end, bool keeplastbyte)
{	return splice(value, begin, end, keeplastbyte);	}
GPT::str::string GPT::str::string::splice_n(size_t begin, size_t end, bool keeplastbyte)
{
	return string(splice(value, begin, end, keeplastbyte));
}

void GPT::str::string::splice_ss(size_t begin, size_t end, bool keeplastbyte)
{
	splice(value, begin, end, keeplastbyte);
}

bool operator== (std::string in, GPT::str::string in2)
{
	return in == in2.get_value();
}

std::string operator+(std::string in, GPT::str::string in2)
{
	return in + in2.value;
}

std::string operator+(GPT::str::string in2, std::string in)
{
	return in2.value + in;
}

std::string operator+(int i, std::string in)
{
	std::string temp = "" + i;
	temp += in;
	return temp;
}

std::ostream& operator<<(std::ostream& out, const GPT::str::string s)
{
	out << s.value;
	return out;
}

//MATH CLASS
int			GPT::Math::rand(int min, int max)					{	return min + std::rand() % (max+1 - min);	};
uint32_t	GPT::Math::rand(uint32_t min,	uint32_t max)	{	return min + std::rand() % (max+1 - min);	};
uint64_t	GPT::Math::rand(uint64_t min,	uint64_t max)	{	return min + std::rand() % (max+1 - min);	};
uint8_t		GPT::Math::rand(uint8_t min,	uint8_t max)	{	return min + std::rand() % (max+1 - min);	};
uint16_t	GPT::Math::rand(uint16_t min,	uint16_t max)	{	return min + std::rand() % (max+1 - min);	};
int64_t		GPT::Math::rand(int64_t min,	int64_t max)	{	return min + std::rand() % (max+1 - min);	};
int8_t		GPT::Math::rand(int8_t min,	int8_t max)		{	return min + std::rand() % (max+1 - min);	};

int GPT::Math::sum(int a, ...)
{
	int sum = a;
	va_list args;
	va_start(args, a);

	int nextArg;
	while ((nextArg = va_arg(args, int)) != 0)
	{
		sum += nextArg;
	}

	va_end(args);
	return sum;
}

float GPT::Math::sum(float a, ...)
{
	float sum = a;
	va_list args;
	va_start(args, a);

	float nextArg;
	while ((nextArg = va_arg(args, double)) != 0)
	{
		sum += nextArg;
	}

	va_end(args);
	return sum;
}

double GPT::Math::sum(double a, ...)
{
	double sum = a;
	va_list args;
	va_start(args, a);

	double nextArg;
	while ((nextArg = va_arg(args, double)) != 0)
	{
		sum += nextArg;
	}

	va_end(args);
	return sum;
}

//MATH::VECTOR CLASS
GPT::Math::Vector::Vector() { size = 0; m_pos = { }; }
GPT::Math::Vector::Vector(size_t size) : size(size), m_pos(size) {}
GPT::Math::Vector::Vector(const std::initializer_list<float> data)
	: size(data.size()), m_pos(size)
{
	const float* it = data.begin();
	for (size_t i = 0; i < size; i++)
		m_pos[i] = it[i];
}

GPT::Math::Vector GPT::Math::Vector::operator+(Vector& other)
{
	assert(other.size == size);
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = other[i] + m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator-(Vector& other)
{
	assert(other.size == size);
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = other[i] - m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator-()
{
	Vector result(size);
	for (size_t i = 0; i < size; i++)
		result[i] = -m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator*(Vector& other)
{
	assert(other.size == size);
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = other[i] * m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator*(int other)
{
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = other * m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator*(float other)
{
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = other * m_pos[i];

	return result;
}
GPT::Math::Vector GPT::Math::Vector::operator/(Vector& other)
{
	assert(other.size == size);
	Vector result(size);

	for (size_t i = 0; i < size; i++)
		result[i] = m_pos[i] / other[i];

	return result;
}

void GPT::Math::Vector::set_dimension(size_t newSize)
{
	size = newSize;
	m_pos.resize(newSize);
}

float& GPT::Math::Vector::operator[](int index)
{
	return m_pos[index];
}
float GPT::Math::Vector::operator[](int index) const
{
	return m_pos[index];
}
size_t GPT::Math::Vector::get_dimension() const
{
	return size;
}
void GPT::Math::Vector::set_position(float first, ...)
{
	m_pos[0] = first;
	va_list args;
	va_start(args, first);
	for (size_t i = 1; i < size; i++) {
		float arg = (float)va_arg(args, double);
		m_pos[i] = arg;
	}
	va_end(args);
}
std::vector<float> GPT::Math::Vector::opposite(bool shouldBeApplied)
{
	std::vector<float> opPos = m_pos;
	for (size_t i = 0; i < size; i++)
	{
		opPos[i] = -opPos[i];
		if (shouldBeApplied == true)
		{
			m_pos[i] = opPos[i];
		}
	}
	return opPos;
}
float GPT::Math::Vector::normalize()
{
	std::vector<float> normalized = m_pos;
	float result = 0;
	for (size_t i = 0; i < size; i++)
	{
		result += (float)std::pow(normalized[i], 2);
	}
	return std::sqrt(result);
}
float GPT::Math::Vector::dot_product(Vector other)
{
	float result = 0.f;
	if (size == other.size)
	{
		for (int i = 0; (size_t)i < get_dimension(); i++)
			result += m_pos[i] * other[i];
		return result;
	}
	return 0;
}
GPT::Math::Vector GPT::Math::Vector::cross_product3D(Vector other)
{
	assert(size == 3 && other.size == 3);
	Vector result(3);
	Vector first(3);
	first[0] = m_pos[0];
	first[1] = m_pos[1];
	if (size == 2) { first[2] = 0; }
	else { first[2] = m_pos[2]; }

	Vector second(3);
	second[0] = other[0];
	second[1] = other[1];
	if (other.size == 2) { second[2] = 0; }
	else { second[2] = other[2]; }

	result[0] = first[1] * second[2] - first[2] * second[1];
	result[1] = first[2] * second[0] - first[0] * second[2];
	result[2] = first[0] * second[1] - first[1] * second[0];

	return result;
}
float GPT::Math::Vector::cross_product2D(Vector other)
{
	assert(size == 2 && other.size == 2);
	float result = m_pos[0] * other[1] - m_pos[1] * other[0];

	return result;
}
//TODO Repair
/*
cross_product_result operator^(Vector other)
{
	assert(size == 2 || size == 3);
	cross_product_result result;
	result.timeToChooseMrFreeman = size == 3;
	if (result.timeToChooseMrFreeman)
		result.vectorMresult = cross_product3D(other);
	else
		result.vector2result = cross_product2D(other);
	return result;
}*/

float GPT::Math::Vector::angle(Vector other)
{
	float result = std::acos(
		dot_product(other) / (normalize(), other.normalize())
	);
	if (cross_product2D(other) > 0)
		return result;
	else
		return -result;
}
float GPT::Math::Vector::distance(Vector other)
{
	//More explanation: One is 2D, one is 3D (for instance).
	assert(size == other.size);

	float result = 0;
	for (int i = 0; (size_t)i < size; i++)
	{
		if (m_pos[i] >= other[i])
			result += (float)std::pow(m_pos[i] - other[i], 2);
		else
			result += (float)std::pow(other[i] - m_pos[i], 2);
	}

	return sqrt(result);
}

//MATH::MATRIX
GPT::Math::Matrix::Matrix(size_t M, size_t N)
{
	size.N = N;
	size.M = M;
	m_data = std::vector<Vector>(M);
	for (size_t i = 0; i < M; i++)
		m_data[i].set_dimension(N);
}

GPT::Math::Matrix::Matrix(const std::initializer_list<Vector>& data)
	: size({ data.size(), data.begin()->get_dimension() }), m_data(size.M)
{
	const Vector* it = data.begin();
	for (size_t i = 0; i < size.M * size.N; i++)
		m_data[i] = it[i];
}

GPT::Math::Vector& GPT::Math::Matrix::operator[](int index)
{
	return m_data[index];
}
GPT::Math::Vector GPT::Math::Matrix::operator[](int index) const
{
	return m_data[index];
}
void GPT::Math::Matrix::set_position(const std::initializer_list<Vector>& data)
{
	const Vector* it = data.begin();

	for (size_t i = 0; i < data.size(); i++) {
		m_data[i] = *it++;
	}
}


GPT::Math::Vector GPT::Math::Matrix::diagonal()
{
	assert(size.M == size.N);

	Vector result(size.M);
	int i = 0;
	for (int m = 0; (size_t)m < size.M; m++)
	{
		for (int n = 0; (size_t)n < size.N; n++)
		{
			if (m == n)
			{
				float t = m_data[m][n];
				result[i++] = t;
			}
		}
	}
	return result;
}

size_t GPT::Math::Matrix::get_sizeN() const
{
	return size.N;
}
size_t GPT::Math::Matrix::get_sizeM() const
{
	return size.M;
}
float GPT::Math::Matrix::trace()
{
	Vector v = diagonal();
	float result = 0;
	for (int i = 0; (size_t)i < v.get_dimension(); i++)
		result += v[i];
	return result;
}
GPT::Math::Matrix GPT::Math::Matrix::opposite()
{
	Matrix m(size.M, size.N);
	for (size_t i = 0; i < size.M; i++)
	{
		m.m_data[i] = m_data[i];
	}
	return m;
}

GPT::Math::Matrix GPT::Math::Matrix::transpose()
{
	Matrix m(size.N, size.M);
	for (int i = 0; (size_t)i < size.N; i++)
		for (int j = 0; (size_t)j < size.M; j++)
			m[j][i] = m_data[i][j];
	return m;
}
GPT::Math::Matrix GPT::Math::Matrix::operator+(Matrix& other)
{
	assert((size.N == other.get_sizeN()) && (size.M == other.get_sizeM()));
	Matrix m(size.M, size.N);
	for (int i = 0; (size_t)i < size.M; i++)
		m[i] = m_data[i] + other[i];
	return m;
}
GPT::Math::Matrix GPT::Math::Matrix::operator*(Matrix& other)
{
	ASSERT((size.N != other.size.M), "Columns of matrix A must equal ligns of matrix B.");
	std::cout << size.M << " " << size.N << std::endl;
	std::cout << other.size.M << ' ' << other.size.N << std::endl;

	Matrix output(other.size.N, size.M);
	for (size_t i = 0; i < m_data.size(); ++i) {
		for (size_t j = 0; j < other.get_sizeN(); ++j) {
			for (size_t k = 0; k < get_sizeN(); ++k) {
				output[i][j] += m_data[i][k] * other[k][j];
			}
		}
	}
	
	return output;
}

GPT::Math::Matrix GPT::Math::Matrix::operator*(int other)
{
	Matrix m(size.M, size.N);
	for (int i = 0; (size_t)i < size.M; i++)
		m[i] = m_data[i] * other;
	return m;
}
GPT::Math::Matrix GPT::Math::Matrix::operator*(float other)
{
	Matrix m(size.M, size.N);
	for (int i = 0; (size_t)i < size.M; i++)
		m[i] = m_data[i] * other;
	return m;
}
float GPT::Math::Matrix::determinant()
{
	assert(size.M == size.N);
	const size_t matrixSize = size.N;

	// If matrix is 1x1
	if (matrixSize == 1)
	{
		return m_data[0][0];
	}

	// If matrix is 2x2
	if (matrixSize == 2)
	{
		return m_data[0][0] * m_data[1][1] - m_data[1][0] * m_data[0][1];
	}

	float result = 0;
	for (int i = 0; (size_t)i < matrixSize; i++)
	{
		// Get the submatrix obtained by deleting the last row and column
		Matrix submatrix(matrixSize - 1, matrixSize - 1);
		for (int j = 1; (size_t)j < matrixSize; j++)
		{
			for (int k = 0; (size_t)k < matrixSize; k++)
			{
				if (k < i)
				{
					submatrix[j - 1][k] = m_data[j][k];
				}
				else if (k > i)
				{
					submatrix[j - 1][k - 1] = m_data[j][k];
				}
			}
		}
		float cofactor = m_data[0][i] * submatrix.determinant();
		result += (i % 2 == 0) ? cofactor : -cofactor;
	}
	return result;
}

GPT::Math::Matrix GPT::Math::Matrix::set_rotation_2D(double rotation)
{
	Matrix result(2, 2);

	result[0][0] = cosf(rotation / (360 / (2 * PI)));
	result[0][1] = -sinf(rotation / (360 / (2 * PI)));

	result[1][0] = sinf(rotation / (360 / (2 * PI)));
	result[1][1] = cosf(rotation / (360 / (2 * PI)));
	return result;
}
GPT::Math::Matrix GPT::Math::Matrix::set_rotation_3D(std::vector<float> rotation)
{
	Matrix result(3, 3);

	result[0][0] =	cosf(rotation[1] / (360 / (2 * PI))) * cosf(rotation[2] / (360 / (2 * PI)));
	result[0][1] = -sinf(rotation[2] / (360 / (2 * PI)));
	result[0][2] =	sinf(rotation[1] / (360 / (2 * PI)));

	result[1][0] =	sinf(rotation[2] / (360 / (2 * PI)));
	result[1][1] =	cosf(rotation[0] / (360 / (2 * PI))) * 1 * cosf(rotation[2] / (360 / (2 * PI)));
	result[1][2] = -sinf(rotation[0] / (360 / (2 * PI)));

	result[2][0] = -sinf(rotation[1] / (360 / (2 * PI)));
	result[2][1] =	sinf(rotation[0] / (360 / (2 * PI)));
	result[2][2] =	cosf(rotation[0] / (360 / (2 * PI))) * cosf(rotation[1] / (360 / (2 * PI)));

	return result;
}
GPT::Math::Matrix GPT::Math::Matrix::set_scale_2D(std::vector<float> scale)
{
	Matrix result(2, 2);

	result[0][0] = scale[0];
	result[1][1] = scale[1];

	return result;
}
GPT::Math::Matrix GPT::Math::Matrix::set_scale_3D(std::vector<float> scale)
{
	Matrix result(3, 3);

	result[0][0] = scale[0];
	result[1][1] = scale[1];
	result[2][2] = scale[2];

	return result;
}


GPT::Math::Matrix GPT::Math::Matrix::set_TRS(std::vector<float> position, std::vector<float> rotation, std::vector<float> scale)
{
	Matrix result(4, 4);

	result[0][0] =	cosf(rotation[1] / (360 / (2 * PI))) * cosf(rotation[2] / (360 / (2 * PI))) * scale[0];
	result[0][1] = -sinf(rotation[2] / (360 / (2 * PI)));
	result[0][2] =	sinf(rotation[1] / (360 / (2 * PI)));
	result[0][3] =	position[0];

	result[1][0] = sinf(rotation[2] / (360 / (2 * PI)));
	result[1][1] = cosf(rotation[0] / (360 / (2 * PI))) * 1 * cosf(rotation[2] / (360 / (2 * PI))) * scale[1];
	result[1][2] = -sinf(rotation[0] / (360 / (2 * PI)));
	result[1][3] = position[1];

	result[2][0] = -sinf(rotation[1] / (360 / (2 * PI)));
	result[2][1] = sinf(rotation[0] / (360 / (2 * PI)));
	result[2][2] = cosf(rotation[0] / (360 / (2 * PI))) * cosf(rotation[1] / (360 / (2 * PI))) * scale[2];
	result[2][3] = position[2];

	result[3][0] = 0;
	result[3][1] = 0;
	result[3][2] = 0;
	result[3][3] = 1;

	return result;
}

int GPT::Math::factorial(int input) {
	ASSERT(input < 0, "Factorial of a negative number, is not possible.");
	int output = 1;

	for (int i = 1; i <= input; ++i) {
		output*=i;
	}
	return output;
}


/*
std::ostream& operator<<(std::ostream& out, const Vector v)
{
for (int i = 0; (size_t)i < v.get_dimension(); i++)
	out << v[i] << " ";
return out;
}*/

//PHY CLASS

GPT::phy::Physics::Physics(uint8_t dimensions, bool shouldBeAffectedByGravity, float gravity)
{
	isAffectedByGravity = shouldBeAffectedByGravity;
	gravityAcceleration = gravity;
	pos.set_dimension(dimensions);
};

void GPT::phy::Physics::Update()
{
	if (isAffectedByGravity)
		pos[verticalDimension] -= gravityAcceleration * deltaTime;
}

//LOG CLASS
std::string GPT::log::GetFormat()
{
	const time_t now = time(0);
	struct tm* ltm;
	ltm = localtime(&now);
	std::string format = "[";
	format += std::to_string(ltm->tm_mday);
	format += "/";
	format += std::to_string(ltm->tm_mon);
	format += "/";
	format += std::to_string(ltm->tm_year + 1900);
	format += " ";
	format += std::to_string(ltm->tm_hour);
	format += ":";
	format += std::to_string(ltm->tm_min);
	format += ":";
	format += std::to_string(ltm->tm_sec);
	format += "] ";
	return format;
}

void GPT::log::OpenFile(std::filesystem::path const& filename, std::ofstream* file)
{/*
	ASSERT(file->is_open(), "Log File is already opened!")

	file->open(filename);
	ASSERT(!file->is_open(), "Log File could not be opened!");
	*file << GetFormat() << "Log file has been opened." << std::endl;
	file->flush();*/
}

void GPT::log::Print(std::ofstream* file, std::string buffer, bool checkNewLine)
{/*
	if (!file->is_open()) return;
	std::cout << GetFormat() << buffer;
	if (checkNewLine && buffer[buffer.size()-1] != '\n')
	{
		*file << GetFormat() << buffer << std::endl;
		std::cout << std::endl;
	}
	else
		*file << GetFormat() << buffer;
	file->flush();*/
}