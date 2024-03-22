import requests
import time

def get_new_tokens(username, password):
    url = 'https://x3oc3nzf94.execute-api.us-east-1.amazonaws.com/test/login'  
    data = {
        'username': username,
        'password': password
    }
    response = requests.post(url, json=data)
    if response.status_code == 200:
        return response.json()['tokens']
    else:
        print(f"Failed to obtain new tokens. Status code: {response.status_code}")
        return None
    
def test_latency_with_new_tokens(url, headers, num_requests):
    latency_times = []
    new_tokens = get_new_tokens('username', 'Password123!') 

    if new_tokens:
        headers['Authorization'] = f'Bearer {new_tokens["RefreshToken"]}'
        for i in range(num_requests):
            start_time = time.time()
            response = requests.get(url, headers=headers)
            end_time = time.time()
            latency = end_time - start_time
            latency_times.append(latency)
            print(f"Request {i+1}: Latency = {latency} seconds")
        
        average_latency = sum(latency_times) / len(latency_times)
        print(f"\nAverage Latency for {num_requests} requests: {average_latency} seconds")
    else:
        print("Failed to obtain new tokens. Unable to test latency.")


url = 'https://x3oc3nzf94.execute-api.us-east-1.amazonaws.com/test/retrievenewtokens'
headers = {
    'Content-Type': 'application/json'
}

num_requests = 10

test_latency_with_new_tokens(url, headers, num_requests)