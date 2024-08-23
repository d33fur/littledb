import asyncio
import socket

SERVER_HOST = '127.0.0.1'
SERVER_PORT = 6378

async def send_request(command):
    reader, writer = await asyncio.open_connection(SERVER_HOST, SERVER_PORT)
    writer.write(command.encode())
    await writer.drain()

    response = await reader.read(100)
    writer.close()
    await writer.wait_closed()
    return response.decode()

async def test_set_get():
    set_response = await send_request('SET key1 value1\n')
    assert set_response == 'OK\n', f"SET command failed: {set_response}"

    get_response = await send_request('GET key1\n')
    assert get_response == 'value1\n', f"GET command failed: {get_response}"

    get_nonexistent_response = await send_request('GET key_nonexistent\n')
    assert get_nonexistent_response == '(nil)\n', f"GET command for nonexistent key failed: {get_nonexistent_response}"

async def run_tests():
    await test_set_get()
    print("All tests passed!")

if __name__ == '__main__':
    asyncio.run(run_tests())
