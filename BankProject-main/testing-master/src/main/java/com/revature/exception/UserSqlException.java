package com.revature.exception;

public class UserSqlException extends RuntimeException {
    public UserSqlException(String message){
        super(message);
    }
}