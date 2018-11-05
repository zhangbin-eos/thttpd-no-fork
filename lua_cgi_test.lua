#! /usr/local/bin/lua


function cgi_web_response_http_header()
	io.write("Content-type: application/json; charset=utf-8\r\n");
	io.write("Cache-Control: no-cache\r\n");
	io.write("Pragma: no-cache\r\n");
	io.write("Expires: 0\r\n");
	io.write("\r\n");
end

function web_response_status(bsuccess, name_buffer, msgbuffer)
	cgi_web_response_http_header();
	io.write("{\"status\":");
	if (bsuccess == 1)
	then
		io.write("\"success\"");
	else
		io.write("\"failed\"");
	end

	if (name_buffer~=nil)
	then
		io.write(",\"name\":\""..name_buffer.."\"");
	end

	if (msgbuffer == nil)
	then
		io.write("}\n\n");
	else
		io.write(",\"data\":{"..msgbuffer.."}}\n\n");
	end
end


web_response_status(1,"cgi-test","lua-cgi");
