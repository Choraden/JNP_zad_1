#include <iostream>
#include <optional>
#include <regex>
#include <map>
#include <sstream>

namespace {
    namespace road {
        using road_type_t = char;
        using road_num_t = int;
        using road_t = std::tuple<road_num_t, road_type_t>;
        using distance_t = int;

        std::optional<road_t> parse_road(const std::string &road_str) {
            static const std::regex road_regex(R"~(^(A|S)([1-9]\d{0,2})$)~");
            std::smatch match;
            if (std::regex_search(road_str, match, road_regex))
                return road_t(road_num_t(std::stoul(match.str(2))), match.str(1)[0]);
            return std::nullopt;
        }

        std::optional<distance_t> parse_distance(const std::string &distance_str) {
            static const std::regex plate_no_regex(R"~(^(0|[1-9]\d*),(\d)$)~");
            std::smatch match;
            if (std::regex_search(distance_str, match, plate_no_regex)) {
                std::string junction = match.str(1);
                int decimal = match.str(2)[0] - '0';
                try {
                    return distance_t(std::stoi(junction) * 10 + decimal);
                } catch (const std::out_of_range &e) {}
            }
            return std::nullopt;
        }
    }

    namespace vehicle {
        using plate_no_t = std::string;
        using vehicle_t = std::tuple<plate_no_t>;

        std::optional<vehicle_t> parse_vehicle(const std::string &plate_no_str) {
            static const std::regex plate_no_regex(R"~(^[a-zA-Z0-9]{3,11}$)~");
            if (std::regex_match(plate_no_str, plate_no_regex))
                return vehicle_t(plate_no_str);
            return std::nullopt;
        }
    }

    namespace input {
        using line_no_t = int;
        using line_t = std::string;
        using line_desc_t = std::tuple<line_no_t, line_t>;
        using line_error_desc_t = line_desc_t;
    }

    namespace toll_charging {
        using road_type_data_t = std::map<road::road_type_t, road::distance_t>;
        using vehicles_data_t = std::map<vehicle::vehicle_t, road_type_data_t>;
        using roads_data_t = std::map<road::road_t, road::distance_t>;

        using not_finished_entry_t = std::tuple<road::road_t, road::distance_t, input::line_desc_t>;
        using not_finished_data_t = std::map<vehicle::vehicle_t, not_finished_entry_t>;

        using state_t = std::tuple<vehicles_data_t, roads_data_t, not_finished_data_t>;

        std::optional<input::line_error_desc_t> add_entry(state_t &state, const vehicle::vehicle_t &vehicle,
                                                          const road::road_t &road, const road::distance_t &distance,
                                                          const input::line_desc_t &line_desc) {
            auto &not_finished = std::get<not_finished_data_t>(state);
            std::optional<input::line_error_desc_t> error_line;
            if (not_finished.count(vehicle)) {
                auto it = not_finished.find(vehicle);
                auto &[not_finished_road, start_distance, paired_line] = it->second;
                if (not_finished_road == road) {
                    auto traveled_distance = std::abs(distance - start_distance);
                    auto &vehicles_data = std::get<vehicles_data_t>(state);
                    auto &roads_data = std::get<roads_data_t>(state);
                    roads_data[road] += traveled_distance;
                    vehicles_data[vehicle][std::get<road::road_type_t>(road)] += traveled_distance;
                    not_finished.erase(it);
                    return std::nullopt;
                } else
                    error_line = paired_line;
            }
            not_finished[vehicle] = not_finished_entry_t(road, distance, line_desc);
            return error_line;
        }
    }

    namespace input {
        using command_desc_t = std::tuple<std::optional<road::road_t>, std::optional<vehicle::vehicle_t>>;
        using info_desc_t = std::tuple<vehicle::vehicle_t, road::road_t, road::distance_t>;

        std::optional<command_desc_t> parse_command(const line_t &line) {
            static const std::regex command_regex(R"~(^\s*\?\s*([^\s]*)\s*$)~");
            std::smatch match;
            if (std::regex_search(line, match, command_regex)) {
                std::string arg = match.str(1);
                if (arg.empty())
                    return command_desc_t(std::nullopt, std::nullopt);
                auto road = road::parse_road(arg);
                auto vehicle = vehicle::parse_vehicle(arg);
                if (!road && !vehicle)
                    return std::nullopt;
                return command_desc_t(road, vehicle);
            }
            return std::nullopt;
        }

        std::vector<std::string> split_args(const std::string &str) {
            std::istringstream ss(str);
            std::string word;
            std::vector<std::string> result;
            while (ss >> word)
                result.emplace_back(std::move(word));
            return result;
        }

        std::optional<info_desc_t> parse_info(const line_t &line) {
            auto args = split_args(line);
            if (args.size() == 3) {
                auto vehicle = vehicle::parse_vehicle(args[0]);
                auto road = road::parse_road(args[1]);
                auto distance = road::parse_distance(args[2]);
                if (!vehicle || !road || !distance)
                    return std::nullopt;
                return info_desc_t(vehicle.value(), road.value(), distance.value());
            }
            return std::nullopt;
        }

        using printable_distance_t = std::tuple<road::distance_t>;

        std::ostream &operator<<(std::ostream &stream, const printable_distance_t &printable_distance) {
            auto distance = std::get<road::distance_t>(printable_distance);
            return stream << distance / 10 << "," << distance % 10;
        }

        std::ostream &operator<<(std::ostream &stream, const road::road_t &road) {
            return stream << std::get<road::road_type_t>(road) << std::get<road::road_num_t>(road);
        }

        std::ostream &operator<<(std::ostream &stream, const vehicle::vehicle_t &vehicle) {
            return stream << std::get<vehicle::plate_no_t>(vehicle);
        }

        std::ostream &operator<<(std::ostream &stream, const toll_charging::road_type_data_t &entries) {
            const char *delimiter = "";
            for (const auto &entry : entries) {
                stream << delimiter << entry.first << " " << printable_distance_t(entry.second);
                delimiter = " ";
            }
            return stream;
        }

        void print_error(const input::line_error_desc_t &error_line) {
            std::cerr << "Error in line " << std::get<input::line_no_t>(error_line) << ": "
                      << std::get<input::line_t>(error_line)
                      << std::endl;
        }

        bool handle_command(toll_charging::state_t &state, const input::line_desc_t &line_desc) {
            auto cmd = input::parse_command(std::get<input::line_t>(line_desc));
            if (!cmd) return false;
            const auto &[cmd_road, cmd_vehicle] = cmd.value();
            const auto &[vehicles_data, roads_data, _] = state;
            if (!cmd_road && !cmd_vehicle) {
                for (const auto &entry : vehicles_data)
                    std::cout << entry.first << " " << entry.second << std::endl;
                for (const auto &entry : roads_data)
                    std::cout << entry.first << " " << printable_distance_t(entry.second) << std::endl;
            }
            if (cmd_vehicle) {
                auto it = vehicles_data.find(cmd_vehicle.value());
                if (it != vehicles_data.end())
                    std::cout << it->first << " " << it->second << std::endl;
            }
            if (cmd_road) {
                auto it = roads_data.find(cmd_road.value());
                if (it != roads_data.end())
                    std::cout << it->first << " " << printable_distance_t(it->second) << std::endl;
            }
            return true;
        }

        bool handle_info(toll_charging::state_t &state, const input::line_desc_t &line_desc) {
            auto info = input::parse_info(std::get<input::line_t>(line_desc));
            if (!info) return false;
            const auto &[vehicle, road, distance] = info.value();
            auto error_line = toll_charging::add_entry(state, vehicle, road, distance, line_desc);
            if (error_line)
                print_error(error_line.value());
            return true;
        }

        void handle_all() {
            toll_charging::state_t state;
            line_no_t line_no = 0;
            line_t line;
            while (std::getline(std::cin, line)) {
                line_no++;
                line_desc_t line_desc(line_no, line);
                if (line.empty() || handle_command(state, line_desc) || handle_info(state, line_desc))
                    continue;
                print_error(line_desc);
            }
        }
    }
}


int main() {
    input::handle_all();
    return 0;
}