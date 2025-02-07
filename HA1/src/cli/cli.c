//
// Created by fhs on 15.12.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <input-validation-loop.h>
#include "cli.h"
#include "parser.h"

Complex askForNumber(const char *position) {
    char input[512];

    size_t promptLength = snprintf(NULL, 0, "What's the %s number?", position);
    char *prompt = malloc(promptLength + 1);
    sprintf(prompt, "What's the %s number?", position);

    validatedInput(
            prompt,
            "Expected a number in the format a+bi",
            validComplexNumber,
            input
    );

    free(prompt);

    return parseComplexNumber(input);
}

static int operatorValidator(const char *input) {
    if (strlen(input) == 1) {
        switch (input[0]) {
            case '+':
            case '-':
            case '*':
            case '/':
                return 1;
        }
    }

    return 0;
}

Operation askForOperation() {
    char input[128];

    validatedInput(
            "What's the operation",
            "Supported operations are '+', '-', '*', and '/'",
            operatorValidator,
            input
    );

    return input[0];
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"

static int formatValidator(const char *input) {
    double choice = atof(input);

    if (choice - (int) choice != 0) {
        return 0;
    }

    return choice >= FORM_RECTANGULAR && choice <= FORM_EXPONENTIAL;
}

Format askForFormat() {
    char input[128];

    validatedInput(
            "In which format do you want to display the result?\n"
            "[1] Rectangular: a+bi\n"
            "[2] Exponential: r*e^(i*Theta)\n"
            "[3]       Polar: r*(cos(Theta)+i*sin(Theta))\n",
            "Expected a whole number between 1 and 3",
            formatValidator,
            input
    );

    input[1] = '\0';
    return atoi(input);
}

#pragma clang diagnostic pop

char *complexToRectangular(Complex number) {
    size_t outputSize = 1;

    char *output = malloc(sizeof(char));
    output[0] = '\0';

    if (number.real || !number.real && !number.imaginary) {
        outputSize += snprintf(output, outputSize, "%f", number.real);

        output = realloc(output, outputSize);

        sprintf(output, "%f", number.real);
    }

    if (number.real && number.imaginary) {
        output = realloc(output, outputSize + 3);

        sprintf(&output[outputSize - 1], number.imaginary > 0 ? " + " : " - ");

        outputSize += 3;
    }

    if (number.imaginary) {
        double imaginary = number.real ? fabs(number.imaginary) : number.imaginary;

        output = realloc(
                output,
                outputSize + snprintf(NULL, 0, "%fi", imaginary)
        );

        sprintf(&output[outputSize - 1], "%fi", imaginary);
    }

    return output;
}

char *complexToString(Format format, Complex number) {
    if (format == FORM_RECTANGULAR) {
        return complexToRectangular(number);
    }

    double r = sqrt(pow(number.real, 2) + pow(number.imaginary, 2));
    double theta = 0;

    double direction = 1 - (2 * (number.imaginary <= 0));

    if (number.real == 0 && number.imaginary != 0) {
        theta = direction * M_PI / 2;
    } else if (number.real < 0) {
        theta = direction * M_PI + atan(number.imaginary / number.real);
    } else if (number.real > 0) {
        theta = atan(number.imaginary / number.real);
        theta = theta ? theta : M_PI;
    }

    char pattern[22];
    if (theta || r) {
        strcpy(pattern, format == FORM_POLAR ? "%f*(cos(%f)+i*sin(%f)" : "%f*e^(%fi)");
    } else {
        strcpy(pattern, "%f");
    }

    size_t patternSize = snprintf(NULL, 0, pattern, r, theta, theta);
    char *result = malloc(patternSize + 1);
    sprintf(result, pattern, r, theta, theta);

    return result;
}
