# export API_KEY=pk_1a067ed03abc41ae9923808ab6694e71
# flask run

import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

app.jinja_env.globals.update(lookup=lookup)
app.jinja_env.globals.update(usd=usd)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    shares = db.execute("SELECT symbol, name, total FROM portfolio WHERE id = :id ORDER BY symbol", id = session["user_id"])
    #price = usd(int(lookup(shares['symbol'])['price']))
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    total = 0
    # would like to find a way not to do total share cost separately like below, but instead in a template
    for row in shares:
        total += lookup(row['symbol'])['price'] * row['total']
    return render_template("index.html", shares = shares, cash = cash[0]['cash'], total = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # receive request and lookup
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 403)
        data = lookup(symbol)
        if data == None:
            return apology("invalid symbol")

        shares = int(request.form.get("shares"))
        # check if didn't enter minus shares
        if int(request.form.get("shares")) <= 0:
            return apology("Are you cheating?")
        # return render_template("cost.html", name=data['name'], price=usd(data['price']) , symbol=data['symbol'])

        # check if affords
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])

        if cash[0]['cash'] < (shares * data['price']):
            return apology("cannot afford that much")
            # if not, return apology

        # check if already exists in portfolio table
        total = db.execute("SELECT total FROM portfolio WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = data['symbol'])
        # if exists, update the value
        if len(total) != 0:
            db.execute("UPDATE portfolio SET total = :total WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = data['symbol'], total = total[0]['total'] + shares)

        else:
            db.execute("INSERT INTO portfolio (id, symbol, name, total) VALUES (:id, :symbol, :name, :total)", id = session["user_id"], symbol = data['symbol'], name = data['name'], total = shares)

        # deduce cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash[0]['cash'] - (shares * data['price']), id = session["user_id"])
        # update transactions table
        db.execute("INSERT INTO transactions (id, symbol, change, price) VALUES (:id, :symbol, :change, :price)", id = session["user_id"], symbol = data['symbol'], change = shares, price = data['price'])
        flash('Bought!')
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    history = db.execute("SELECT timestamp, symbol, change, price FROM transactions WHERE id = :id", id = session["user_id"])
    return render_template("history.html", history = history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash('Logged in!')
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    flash('Logged out!')
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("must provide a symbol", 403)
        data = lookup(symbol)
        if data == None:
            return apology("invalid symbol")
        return render_template("cost.html", name=data['name'], price=usd(data['price']) , symbol=data['symbol'])
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password matches
        elif request.form.get("password") != request.form.get("password_check"):
            return apology("password doesn't match", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))

        flash('Registered!')
        return redirect("/")


    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not symbol:
            return apology("No symbol selected")
        usershares = db.execute("SELECT total FROM portfolio WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = request.form.get("symbol"))
        if usershares[0]['total'] < int(shares):
            return apology("You don't have that many shares")
        if int(shares) <= 0:
            return apology("Are you cheating?")

        data = lookup(symbol)
        if data == None:
            return apology("invalid symbol")
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        total = db.execute("SELECT total FROM portfolio WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)
        db.execute("UPDATE portfolio SET total = :total WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol, total = total[0]['total'] - shares)
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cash[0]['cash'] - (shares * data['price']), id = session["user_id"])
        db.execute("INSERT INTO transactions (id, symbol, change, price) VALUES (:id, :symbol, :change, :price)", id = session["user_id"], symbol = data['symbol'], change = -shares, price = data['price'])
        flash('Sold!')
        return redirect("/")

    else:
        usershares = db.execute("SELECT symbol, total FROM portfolio WHERE id = :id", id = session["user_id"])
        return render_template("sell.html", usershares = usershares)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
