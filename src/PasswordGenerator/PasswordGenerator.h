#include <string>

struct GeneratorSettings
{
	bool Lower;
	bool Upper;
	bool Digit;
	bool Symbol;
};

class PasswordGenerator
{
public:
	static std::string GeneratePass(const int& Length, const GeneratorSettings& Settings);
protected:
private:
};