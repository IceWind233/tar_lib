#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "tarlib.hpp"

#include <iostream>
#include <sstream>
#include <experimental/filesystem>

Tar::Tar(const char file_name[]) : Tar(std::string(file_name)) {}

Tar::Tar(const std::string& file_name) {
	read_file(file_name);
}

Tar::Tar(const char bytes[], size_t size) {
	bytes2tarObjs(std::string(bytes, bytes + size), tar_headers_);
}

void Tar::write_file(const std::string& path) const {

	for (auto& idx : tar_headers_) {
		create_file(idx, path);
	}
}

void Tar::print() const {
	for (auto i : tar_headers_) {
		std::cout << std::string(i->file_name, i->file_name + 100);
	}
}

size_t Tar::get_size(size_t index) const {
	if (index < tar_headers_.size()) {
		 return octStr2int(std::string(tar_headers_[index]->file_size, tar_headers_[index]->file_size + 12));
	}else {
		throw "Index out of range";
	}
}

void Tar::get_bytes(size_t index, u_char_t* dst) {
	if (index < tar_headers_.size()) {
		tarobj2bytes(tar_headers_[index], dst);
	}
	else {
		throw "Index out of range";
	}
}

void Tar::read_file(const std::string& file_name) {
	auto tar_file = std::make_shared<std::ifstream>(file_name, std::ios::binary);

	if (!tar_file->is_open()) {
		return;
	}

	std::ostringstream tar_data;
	tar_data << tar_file->rdbuf();

	bytes2tarObjs(tar_data.str(), tar_headers_);
}

void Tar::bytes2tarObjs(
	const std::string& src,
	std::vector<std::shared_ptr<tar_header>>& dst
	) {

	for (auto idx = src.begin(); idx < src.end() - 1024;) {
		auto tmp = std::shared_ptr<tar_header>(new tar_header);
		const auto segment_len = round_up(
			octStr2int(
					std::string(idx + 124, idx + 136)
				),
			tar_header::kTarHeaderSize)
		+ tar_header::kTarHeaderSize;

		bytes2tarObj(std::string(idx, idx + segment_len), tmp);

		idx += segment_len;

		dst.push_back(tmp);
	}
}

void Tar::bytes2tarObj(
	const std::string& src,
	std::shared_ptr<tar_header>& dst
	) {
	auto idx = src.begin();
	
	/* copy header form it idx to dst */
	/* idk how to copy!!!! @StevenRCE0 */
	while (idx != src.begin() + tar_header::kTarHeaderSize) {
		dst->file_name[idx - src.begin()] = *idx;
		++idx;
	}
	
	const auto file_size = octStr2int(std::string(dst->file_size, dst->file_size + 12));
	if (file_size) {
		const std::shared_ptr<char[]> data(new char[file_size], std::default_delete<char>());
		/* copy the content of file size from idx to dst */
		while (idx != src.begin() + tar_header::kTarHeaderSize + file_size) {
			data[idx - src.begin() - tar_header::kTarHeaderSize] = *idx;
			++idx;
		}
		dst->file_data = data;
	}
}

void Tar::tarobj2bytes(const std::shared_ptr<tar_header>& src, const u_char_t* dst) {
	const auto file_size = octStr2int(std::string(src->file_size, src->file_size + 12));
	memcpy(src->file_data.get(), dst, file_size);
}

size_t Tar::octStr2int(const std::string& oct_str) {
	size_t result = 0;

	for (auto idx = oct_str.begin(); idx != oct_str.end() - 1; ++idx) {
		result = result * 8 + (*idx - '0');
	}

	return result;
}

size_t Tar::round_up(size_t num, size_t incr) {
	return static_cast<size_t>(ceil(num / static_cast<double>(incr))) * incr;
}

void Tar::create_file(const std::shared_ptr<tar_header>& src, const std::string& path) const {
	auto out_file = std::make_shared<std::ofstream>(
		path + std::string(src->file_name, src->file_name + 100),
		std::ios::binary);
	std::cout << path + std::string(src->file_name, src->file_name + 100) << std::endl;

	if(src->file_type[0] == '5') {
		std::experimental::filesystem::create_directory(path + std::string(src->file_name, src->file_name + 100));
		return;
	}

	out_file->write(src->file_data.get(),
		octStr2int(std::string(src->file_size, src->file_size + 12)));
}


