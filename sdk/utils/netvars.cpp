#include "netvars.h"

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <chrono>
#include "filesystem"

#include "../interfaces.h"

namespace netvar_manager {
	using netvar_key_value_map = std::unordered_map< uint32_t, uintptr_t >;
	using netvar_table_map = std::unordered_map< uint32_t, netvar_key_value_map >;
	void initialize_props(netvar_table_map& table_map);

	static netvar_table_map map = {};

	uintptr_t get_net_var(const uint32_t table,
		const uint32_t prop) {

		if (map.empty())
			initialize_props(map);

		if (map.find(table) == map.end())
			return 0;

		netvar_key_value_map& table_map = map.at(table);
		if (table_map.find(prop) == table_map.end())
			return 0;

		return table_map.at(prop);
	}

	void add_props_for_table(netvar_table_map& table_map, const uint32_t table_name_hash, const std::string& table_name, recv_table* table, const bool dump_vars, std::map< std::string, std::map< uintptr_t, std::string > >& var_dump, const size_t child_offset = 0) {
		for (auto i = 0; i < table->props_count; ++i) {
			auto& prop = table->props[i];


			if (prop.data_table && prop.elements_count > 0) {
				if (std::string(prop.prop_name).substr(0, 1) == std::string("0"))
					continue;

				add_props_for_table(table_map, table_name_hash, table_name, prop.data_table, dump_vars, var_dump, prop.offset + child_offset);
			}

			auto name = std::string(prop.prop_name);

			if (name.substr(0, 1) != "m" /*&& name.substr( 0, 1 ) != "b"*/)
				continue;

			const auto name_hash = fnv::hash(prop.prop_name);
			const auto offset = uintptr_t(prop.offset) + child_offset;

			try
			{
				table_map[table_name_hash][name_hash] = offset;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}


			if (dump_vars)
			{
				var_dump[table_name][offset] = prop.prop_name;
			}
		}
	}

	void initialize_props(netvar_table_map& table_map) {
		const auto dump_vars = true;

		std::map<std::string, std::map<uintptr_t, std::string>> var_dump;

		for (auto client_class = interfaces::client->get_client_class();
			client_class;
			client_class = client_class->next_ptr) {

			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			if (!table || !table->table_name)
				continue;

			const auto table_name = table->table_name;
			const auto table_name_hash = fnv::hash(table_name);

			add_props_for_table(table_map, table_name_hash, table_name, table, dump_vars, var_dump);
			}

		std::cout << "\n\n==== NetVar Dump ====\n";
		for (const auto& [table_name, props] : var_dump) {
			std::cout << "\n[ " << table_name << " ]\n";
			for (const auto& [offset, name] : props) {
				std::cout << "  0x" << std::hex << offset << " : " << name << "\n";
			}
		}
		std::cout << "==== End Dump ====\n\n";
	}

	void create_directory_if_needed(const std::string& directory_path) {
		if (!std::filesystem::exists(directory_path)) {
			if (!std::filesystem::create_directories(directory_path)) {
				std::cerr << "Failed to create directory: " << directory_path << std::endl;
			}
		}
	}

	std::string get_current_timestamp() {
		auto now = std::chrono::system_clock::now();
		std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm = *std::localtime(&now_time_t);

		std::stringstream ss;
		ss << std::put_time(&now_tm, "%Y-%m-%d @ %H-%M-%S");
		return ss.str();
	}

	void dump_netvars_to_file(const std::string& directory) {
		const auto dump_vars = true;

		create_directory_if_needed(directory);

		std::string timestamp = get_current_timestamp();
		std::string filename = directory + "\\" + timestamp + ".txt";

		std::map<std::string, std::map<uintptr_t, std::string>> var_dump;
		for (auto client_class = interfaces::client->get_client_class();
			 client_class;
			 client_class = client_class->next_ptr) {

			const auto table = reinterpret_cast<recv_table*>(client_class->recvtable_ptr);
			if (!table || !table->table_name)
				continue;

			const auto table_name = table->table_name;
			const auto table_name_hash = fnv::hash(table_name);

			netvar_manager::add_props_for_table(netvar_manager::map, table_name_hash, table_name, table, dump_vars, var_dump);
			 }

		std::ofstream dump_file(filename);
		if (!dump_file.is_open()) {
			std::cerr << "[ERROR] Failed to open netvar dump file.\n";
			return;
		}

		dump_file << "==== NetVar Dump ====\n";
		for (const auto& [table_name, props] : var_dump) {
			dump_file << "\n[ " << table_name << " ]\n";
			for (const auto& [offset, name] : props) {
				dump_file << "  0x" << std::hex << offset << " : " << name << "\n";
			}
		}
		dump_file << "==== End Dump ====\n";
		dump_file.close();

		std::cout << "[INFO] Netvars dumped to file: " << filename << "\n";
	}

	void init_netvar_manager()
	{
		if (map.empty())
			initialize_props(map);
	}
}