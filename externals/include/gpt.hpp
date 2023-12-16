#pragma once
/*
 *	GPT - General Purpose Toolbox
 *	You can disable different parts.
 *	- String	: #define GPT_DISABLE_STRING
 *	- Math		: #define GPT_DISABLE_MATH
 *	- Physics	: #define GPT_DISABLE_PHYSICS |	Physics require math to be enabled.
 *	- Logs		: #define GPT_DISABLE_LOG
 *
 * -------------------------------------------
 * What it includes by default:
 * string, vector, cstdarg, cmath
 * cassert, ostream, iomanip, iostream
 * filestystem
 * -------------------------------------------
 * LOG EXPLANATION
 * Log class is static and do not need to initialized as an
 * object.
 *
 * Init:
 * Create a std::ofstream, you can open it your way, or
 * using GPT::Log::OpenFile("your path", &your_file);
 *
 * Usage:
 * You can use GPT::Log::Print(&your_file, your_string);
 * to print in the console and in the file.
 * Date format is automatic, edit the function GPT::Log::GetFormat()
 * if it does not please you.
*/

#include <string>
#include <vector>
#include <cstdarg>
#include <cmath>
#include <cassert>
#include <ostream>
#include <iomanip>
#include <iostream>
//#include <filesystem>

//ASSERT
#ifdef _IOSTREAM_
	#ifdef _DEBUG
		#define ASSERT(a,b) {if (a) { std::cout << (b) << std::endl; __debugbreak();}}
	#else
		#define ASSERT(a,b) if (a) { throw std::runtime_error(b);}
	#endif
#else
	#ifdef _DEBUG
		#define ASSERT(a,b) {if (a) { __debugbreak();}}
	#else
		#define ASSERT(a,b) if (a) {throw std::runtime_error(b);}
	#endif
#endif

class GPT {
public:
	//String utility
	#ifndef GPT_DISABLE_STRING
	class str {
		public:
		//Returns the input string in lowercase.
		static std::string lowercase(std::string in);

		//Returns the input string in uppercase.
		static std::string uppercase(std::string in);

		//Returns the first word of input string.
		//First word is separated by a space, a \n, or a \0.
		static std::string getfirstword(std::string in);

		//TODO
		//Returns if input string contains input word.
		static std::string includes(std::string in, std::string word);

		//Splits a string into an array of substrings
		static void split(std::string in, char splitter, std::vector<std::string>* output);

		static bool startsWith(std::string input, std::string start);
		static bool endsWith(std::string input, std::string end);

		static std::string splice(std::string input, size_t begin = 0, size_t end = 0, bool keeplastbyte = true);

		//--------------------------------
		//GPT::str::string
		class string {
			public:
				//The std::string value
				std::string value;


				string();
				string(std::string in);
				string(const char in);
				string(const char* in);

				std::string operator=(string in);
				bool		operator==(string in);
				string		operator+(int in);
							operator std::string();
				string		operator=(const char input);
				char&		operator[](int index);
				char		operator[](int index) const;
				string*		operator=(const char* input);


				//Gets std::string size.
				size_t 			size();

				//Gets content (std::string)
				std::string		get_value();

				//Returns the input string in lowercase.
				void lowercase_n();

				//Returns the input string in uppercase.
				void uppercase_n();
				//Returns the first word of input string.
				//First word is separated by a space, a \n, or a \0.
				//Returns a std::script
				std::string getfirstword_n();
				//Returns the first word of input string.
				//First word is separated by a space, a \n, or a \0.
				//Returns a GPT::str::string
				string getfirstword_nn();

				//Splits this GPT::str::string into an array of sub std::string
				void split_n(char splitter, std::vector<std::string>* output);

				//Splits this GPT::str::string into an array of sub GPT::str::string
				void split_n(char splitter, std::vector<string>* output);

				bool startsWith_n(std::string start);

				//GPT::str::splice(), but input is the value of this.
				std::string splice_s(size_t begin = 0, size_t end = 0, bool keeplastbyte = true);

				//GPT::str::splice() but returns GPT::str::string
				string splice_n(size_t begin = 0, size_t end = 0, bool keeplastbyte = true);

				//GPT::str::splice() but acts only on this object
				void splice_ss(size_t begin = 0, size_t end = 0, bool keeplastbyte = true);

		};
	};
	#endif	//GPT_DISABLE_STRING

	//Math utility
	#ifndef GPT_DISABLE_MATH
	class Math
	{
		public:
		#define PI 3.1415926535
		#include <cstdlib>
		#include <cstdarg>
		static int		rand(int min, int max)					;
		static uint32_t	rand(uint32_t min,	uint32_t max)	;
		static uint64_t	rand(uint64_t min,	uint64_t max)	;
		static uint8_t	rand(uint8_t min,	uint8_t max)	;
		static uint16_t rand(uint16_t min,	uint16_t max)	;
		static int64_t	rand(int64_t min,	int64_t max)	;
		static int8_t	rand(int8_t min,	int8_t max)		;

		static int		factorial(int input);

		static int		sum(int, ...);
		static float	sum(float, ...);
		static double	sum(double, ...);

		struct Vector2
		{
			float x = 0; float y = 0;
		};
		struct Vector3
		{
			float x = 0; float y = 0; float z = 0;
		};
		struct Vector4
		{
			float x = 0; float y = 0; float z = 0; float w = 0;
		};

		class Vector
		{
		private:
			size_t size;
			std::vector<float> m_pos;

		public:
			Vector();
			Vector(size_t size);
			Vector(const std::initializer_list<float> data);
			Vector operator+(Vector& other);
			Vector operator-(Vector& other);
			Vector operator-();
			Vector operator*(Vector& other);
			Vector operator*(int other);
			Vector operator*(float other);
			Vector operator/(Vector& other);

			void set_dimension(size_t newSize);

			float& operator[](int index);
			float operator[](int index) const;
			size_t get_dimension() const;
			void set_position(float first, ...);
			std::vector<float> opposite(bool shouldBeApplied = false);
			float normalize();
			float dot_product(Vector other);
			Vector cross_product3D(Vector other);
			float cross_product2D(Vector other);
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

			float angle(Vector other);
			float distance(Vector other);

		};

		struct cross_product_result
		{
			Vector  vectorMresult = { 0,0,0 };
			float   vector2result = 0.f;
			bool    timeToChooseMrFreeman = false;
		};
		
		friend std::ostream& operator<<(std::ostream& out, const Vector v)
		{
			for (int i = 0; (size_t)i < v.get_dimension(); i++)
				out << v[i] << " ";
			return out;
		}

		class Matrix
		{
		private:
			struct size
			{
				size_t M;
				size_t N;
			} size;
			std::vector<Vector> m_data;
		public:

			Matrix(size_t M, size_t N);

			Matrix(const std::initializer_list<Vector>& data);
			Vector& operator[](int index);
			Vector operator[](int index) const;
			void set_position(const std::initializer_list<Vector>& data);
			
			friend std::ostream& operator<<(std::ostream& out, const Matrix m)
			{
				out << "\n";
				out << std::setprecision(3);
				for (int i = 0; (size_t)i < m.get_sizeM(); i++)
				{
					out << m[i] << std::endl;
				}
				return out;
			}
			Vector diagonal();

			size_t get_sizeN() const;
			size_t get_sizeM() const;
			float trace();
			Matrix opposite();
			Matrix transpose();
			Matrix operator+(Matrix& other);
			Matrix operator*(Matrix& other);

			Matrix operator*(int other);
			Matrix operator*(float other);
			float determinant();

			Matrix set_rotation_2D(double rotation);
			Matrix set_rotation_3D(std::vector<float> rotation);
			Matrix set_scale_2D(std::vector<float> scale);
			Matrix set_scale_3D(std::vector<float> scale);


			Matrix set_TRS(std::vector<float> position, std::vector<float> rotation, std::vector<float> scale);
		};

	};
	#endif	//GPT_DISABLE_MATH

	//Physics utility
	#ifndef GPT_DISABLE_PHYSICS
	#ifndef GPT_DISABLE_MATH
	class phy
	{
		public:
		#define GPT_GRAVITY	9.81f
		class Physics
		{
		public:
			GPT::Math::Vector	pos;
			float deltaTime = 0; //Provide the deltaTime to it. It does not update by itself.
			bool	isAffectedByGravity = true;
			float	gravityAcceleration = GPT_GRAVITY;
			uint8_t verticalDimension = 1;

			Physics(uint8_t dimensions, bool shouldBeAffectedByGravity = false, float gravity = GPT_GRAVITY);

			void Update();

		};
	};
	#endif	//GPT_DISABLE_MATH
	#endif	//GPT_DISABLE_PHYSICS


	//Log utility
	#define GPT_DISABLE_LOG
	#ifndef GPT_DISABLE_LOG
	class log {

		public:

			static std::string GetFormat();

			static void OpenFile(std::filesystem::path const& filename, std::ofstream* file);

			/*
			static void Print(std::ofstream* file, const char* format, ...)
			{
				va_list args;
				va_start(args, format);
				char *buffer;
				int len = vsnprintf(buffer, 1024, format, args) + 1; // terminating '\0'
				buffer = (char*)malloc(len * sizeof(char));
				vsnprintf(buffer, len, format, args);
				std::cout << buffer;
				if (file->is_open())
					*file << GetFormat() << buffer << std::endl;

				va_end(args);
			}*/

			static void Print(std::ofstream* file, std::string buffer, bool checkNewLine = true);
	};
	#endif	//GPT_DISABLE_LOG

};
#ifndef GPT_DISABLE_STRING
	bool operator== (std::string in, GPT::str::string in2);
	#ifndef GPT_GUARD
	#define GPT_GUARD
	bool operator== (std::string in, GPT::str::string in2);

	std::string operator+(std::string in, GPT::str::string in2);

	std::string operator+(GPT::str::string in2, std::string in);

	std::string operator+(int i, std::string in);

	std::ostream& operator<<(std::ostream& out, const GPT::str::string s);

	#endif

#endif