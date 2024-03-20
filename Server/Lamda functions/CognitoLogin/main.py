import boto3
import os
import sys

USER_POOL_APP_CLIENT_ID = '3rtk9uvfe89ge61tu6vignmakl'

client = boto3.client('cognito-idp')

def lambda_handler(event, context):
    if 'username' not in event or 'password' not in event:
        return {
            'status': 'fail',
            'msg': 'Username and password are required'
        }
    resp, msg = initiate_auth(event['username'], event['password'])
    if msg != None:
        return {
            'status': 'fail', 
            'msg': msg
        }
    response = {
        'status': 'success',
        'tokens': resp['AuthenticationResult']
    }
    return response

def initiate_auth(username, password):
    try:
        resp = client.initiate_auth(
            ClientId=USER_POOL_APP_CLIENT_ID,
            AuthFlow='USER_PASSWORD_AUTH',
            AuthParameters={
                'USERNAME': username,
                'PASSWORD': password
            })
    except client.exceptions.InvalidParameterException as e:
        print(e)
        return None, "Username and password must not be empty"
    except (client.exceptions.NotAuthorizedException, client.exceptions.UserNotFoundException) as e:
        return None, "Username or password is incorrect"
    except Exception as e:
        print("Uncaught exception:", e, file=sys.stderr)
        return None, "Unknown error"
    return resp, None