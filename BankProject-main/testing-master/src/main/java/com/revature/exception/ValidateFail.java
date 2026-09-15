package com.revature.exception;

public class ValidateFail extends RuntimeException {
    public ValidateFail(String message) {
        super(message);
    }
}