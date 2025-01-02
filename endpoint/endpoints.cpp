#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

// The calling sequence of the functions is as follows:
// Server: server_end_point => create_acceptor_socket => bind_acceptor_socket => accept_new_connection
// Client: client_end_point => create_tcp_socket => connect_to_end / dns_connect_to_end

int client_end_point() {
	// Step 1. Assume that the client application has already 
	// obtained the IP-address and the protocol port number.
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	boost::system::error_code ec;

	// Step 2. Using IP protocol version independent address
	// representation.
	asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to parse the IP address. Error code = " << ec.value() << ". Message is " << ec.message() << std::endl;
		return ec.value();
	}

	// Step 3. Create an endpoint that represents the server
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

int server_end_point() {
	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = 3333;

	// Step 2. Create special object of asio::ip::address class
	// that specifies all IP-addresses available on the host. Note
	// that here we assume that server works over IPv6 protocol.
	asio::ip::address ip_address = asio::ip::address_v6::any();

	// Step 3. Create object of asio::ip::tcp class representing
	asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

int create_tcp_socket() {
	// Step 1. An instance of 'io_service' class is required by
	// socket constructor. 
	asio::io_context ios;

	// Step 2. Creating an object of 'tcp' class representing
	// a TCP protocol with IPv4 as underlying protocol.
	asio::ip::tcp protocal = asio::ip::tcp::v4();

	// Step 3. Instantiating an active TCP socket object.
	asio::ip::tcp::socket sock(ios);
	boost::system::error_code ec;

	// Step 4. Opening the socket.
	sock.open(protocal, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to open the socket. Error code = " << ec.value() << ". Message is " << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
}

int create_acceptor_socket() {
	// Step 1. An instance of 'io_service' class is required by
	// socket constructor. 
	asio::io_context ios;

	// Step 2. Creating an object of 'tcp' class representing
	// a TCP protocol with IPv6 as underlying protocol.
	asio::ip::tcp protocal = asio::ip::tcp::v6();

	// Step 3. Instantiating an acceptor socket object.
	asio::ip::tcp::acceptor acceptor(ios);
	boost::system::error_code ec;

	// Step 4. Opening the acceptor socket.
	acceptor.open(protocal, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to open the acceptor socket. Error code = " << ec.value() << ". Message is " << ec.message() << std::endl;
		return ec.value();
	}

	// new version - simpler
	/*asio::ip::tcp::acceptor a(ios, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 3333));*/
	return 0;
}

int bind_acceptor_socket() {
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port_num);
	asio::io_context ios;
	asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
	boost::system::error_code ec;
	acceptor.bind(ep, ec);
	if (ec.value() != 0) {
		std::cout << "Failed to bind the acceptor socket. Error code = " << ec.value() << ". Message is " << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
}

int connect_to_end() {
	std::string raw_ip_address = "192.168.1.124";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ios;
		asio::ip::tcp::socket sock(ios, ep.protocol());
		sock.connect(ep);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int dns_connect_to_end() {
	std::string host = "llfc.club";
	std::string port_num = "3333";
	asio::io_context ios;
	asio::ip::tcp::resolver::query resolver_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);
	asio::ip::tcp::resolver resolver(ios);
	try {
		asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query);
		asio::ip::tcp::socket sock(ios);
		asio::connect(sock, it);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int accept_new_connection() {
	const int BACKLOG_SIZE = 30;
	unsigned short port_num = 3333;
	asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
	asio::io_context ios;
	try {
		asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
		acceptor.bind(ep);
		acceptor.listen(BACKLOG_SIZE);
		asio::ip::tcp::socket sock(ios);
		acceptor.accept(sock);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

void use_const_buffer() {
	std::string buf = "hello world";
	asio::const_buffer asio_buf(buf.c_str(), buf.length());
	std::vector<asio::const_buffer> buffers_sequence;
	buffers_sequence.push_back(asio_buf);
}

void use_buffer_str() {
	asio::const_buffers_1 output_buf = asio::buffer("hello world");
}

void use_buffer_array() {
	const size_t BUF_SIZE_BYTES = 30;
	std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
	auto input_buf = asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
}

void write_to_socket(asio::ip::tcp::socket& sock) {
	std::string buf = "hello world";
	std::size_t total_bytes_written = 0;
	while (total_bytes_written != buf.length()) {
		total_bytes_written += sock.write_some(asio::buffer(buf.c_str() + total_bytes_written, buf.length() - total_bytes_written));
	}
}

int send_data_by_write_some() {
	std::string raw_ip_address = "192.168.3.11";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		write_to_socket(sock);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
	}
	return 0;
}

int send_data_by_send() {
	std::string raw_ip_address = "192.168.3.11";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		std::string buf = "Hello World";
		int send_length = sock.send(asio::buffer(buf.c_str(), buf.length()));
		if (send_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
	}
	return 0;
}

int send_data_by_write() {
	std::string raw_ip_address = "192.168.3.11";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		std::string buf = "Hello World";
		int send_length = asio::write(sock, asio::buffer(buf.c_str(), buf.length()));
		if (send_length <= 0) {
			return 0;
		}
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
	}
	return 0;
}

std::string read_from_socket(asio::ip::tcp::socket& sock) {
	const unsigned char MESSAGE_SIZE = 7;
	char buf[MESSAGE_SIZE];
	std::size_t total_bytes_read = 0;
	while (total_bytes_read != MESSAGE_SIZE) {
		total_bytes_read += sock.read_some(asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read));
	}
	return std::string(buf, total_bytes_read);
}

int read_data_by_read_some() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		read_from_socket(sock);
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int read_data_by_receive() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		const unsigned char BUFF_SIZE = 7;
		char buffer_receive[BUFF_SIZE];
		int receive_length = sock.receive(asio::buffer(buffer_receive, BUFF_SIZE));
		if (receive_length <= 0)
			std::cout << "receive failed" << std::endl;
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}

int read_data_by_read() {
	std::string raw_ip_address = "127.0.0.1";
	unsigned short port_num = 3333;
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
		asio::io_context ioc;
		asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
		const unsigned char BUFF_SIZE = 7;
		char buffer_receive[BUFF_SIZE];
		int receive_length = asio::read(sock, asio::buffer(buffer_receive, BUFF_SIZE));
		if (receive_length <= 0)
			std::cout << "receive failed" << std::endl;
	}
	catch (system::system_error& e) {
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;
		return e.code().value();
	}
	return 0;
}