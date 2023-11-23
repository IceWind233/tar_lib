#ifndef TARLIB_HPP
#define TARLIB_HPP

#include <memory>
#include <string>
#include <fstream>
#include <vector>

struct tar_header {
	using u_char_t = unsigned char;
	static const size_t kTarHeaderSize = 512;

	u_char_t file_name	[100];
	u_char_t file_mode	[8];
	u_char_t owner_id	[8];
	u_char_t group_id	[8];
	u_char_t file_size	[12];
	u_char_t lmt		[12]; // aka Last modification time in numeric Unix time format
	u_char_t check_sum	[8];
	u_char_t file_type	[1];
	u_char_t link_name	[100];
	u_char_t _padding[255];

	std::shared_ptr<char[]> file_data;
};

class Tar {

	using u_char_t = unsigned char;

public:
	Tar(const char file_name[]);

	Tar(const std::string& file_name);

	Tar(const char bytes[], size_t size);

public:

	void write_file(const std::string& path) const;

	void print() const;

	size_t get_size(size_t index) const;

	void get_bytes(size_t index, u_char_t* dst);

	std::string get_name(size_t index) const;

	bool is_folder(size_t index) const;

private:

	void read_file(const std::string&);

	static void bytes2tarObjs(
		const std::string& src,
		std::vector<std::shared_ptr<tar_header>>& dst
	);

	static void bytes2tarObj(
		const std::string& src,
		std::shared_ptr<tar_header>& dst
	);

	static void tarobj2bytes(
		const std::shared_ptr<tar_header>& src,
		const u_char_t* dst
	);

	static size_t octStr2int(const std::string& oct_str);

	static size_t round_up(size_t num, size_t incr);

	void create_file(const std::shared_ptr<tar_header>& src, const std::string& path) const;

private:

	std::vector<std::shared_ptr<tar_header>> tar_headers_;

};

#endif