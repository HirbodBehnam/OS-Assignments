import asyncio
import time

import httpx


async def async_get(client: httpx.AsyncClient, url: str, **kwargs):
    """
    This is an async function that gets a url.
    """
    try:
        resp = await client.request(method="GET", url=url, **kwargs)
        return resp.text
    except Exception as e:
        pass



async def main():
    """
    This function asynchronously gets the url "http://example.org" 20 times; i.e. it requests that page 20 times without waiting for the response to come back.
    """
    number_of_times = 20
    start_time = time.time()
    async with httpx.AsyncClient() as client:
        answers = await asyncio.gather(*(async_get(client, "http://www.example.org") for i in range(number_of_times)))
    end_time = time.time()
    print(end_time - start_time)

if __name__ == '__main__':
    """
    Driver code
    """
    asyncio.run(main())
