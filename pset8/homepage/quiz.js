function unlock(){
    document.getElementById("home").classList.remove('disabled');
    document.getElementById("cv").classList.remove('disabled');
    document.getElementById("photos").classList.remove('disabled');
}

(function(){
  function buildQuiz(){
    // variable to store the HTML output
    const output = [];

    // for each question...
    myQuestions.forEach(
      (currentQuestion, questionNumber) => {

        // variable to store the list of possible answers
        const answers = [];

        // and for each available answer...
        for(letter in currentQuestion.answers){

          // ...add an HTML radio button
          //removed ${letter} :
          answers.push(
            `<label>
              <input type="radio" name="question${questionNumber}" value="${letter}">

              ${currentQuestion.answers[letter]}
            </label>`
          );
        }

        // add this question and its answers to the output
        output.push(
          `<div class="question"> ${currentQuestion.question} </div>
          <div class="answers"> ${answers.join("<br>")} </div>`
        );
      }
    );

    // finally combine our output list into one string of HTML and put it on the page
    quizContainer.innerHTML = output.join('');
  }

  function showResults(){

    // gather answer containers from our quiz
    const answerContainers = quizContainer.querySelectorAll('.answers');

    // keep track of user's answers
    let numCorrect = 0;

    // for each question...
    myQuestions.forEach( (currentQuestion, questionNumber) => {

      // find selected answer
      const answerContainer = answerContainers[questionNumber];
      const selector = `input[name=question${questionNumber}]:checked`;
      const userAnswer = (answerContainer.querySelector(selector) || {}).value;

      // if answer is correct
      if(userAnswer === currentQuestion.correctAnswer){
        // add to the number of correct answers
        numCorrect++;

        // color the answers green
        answerContainers[questionNumber].style.color = 'lightgreen';
      }
      // if answer is wrong or blank
      else{
        // color the answers red
        answerContainers[questionNumber].style.color = 'darkred';
      }
    });

    // show number of correct answers out of total
    resultsContainer.innerHTML = `${numCorrect} out of ${myQuestions.length}`;
    if (numCorrect == myQuestions.length)
    {
        unlock()
        resultsContainer.innerHTML = `You are free!`;
    }
  }

  const quizContainer = document.getElementById('quiz');
  const resultsContainer = document.getElementById('results');
  const submitButton = document.getElementById('submit');
  const myQuestions = [
    {
      question: "In which year was I born?",
      answers: {
        a: "1994",
        b: "1995",
        c: "1996"
      },
      correctAnswer: "b"
    },
    {
      question: "For which school's course have I done this project?",
      answers: {
        a: "Yale",
        b: "Cambridge",
        c: "Harvard"
      },
      correctAnswer: "c"
    },
    {
      question: "What genre of photos have I taken the most?",
      answers: {
        a: "Portrait",
        b: "Architecture",
        c: "Nature",
        d: "Animal"
      },
      correctAnswer: "c"
    }
  ];

  // Kick things off
  buildQuiz();

  // Event listeners
  submitButton.addEventListener('click', showResults);
})();