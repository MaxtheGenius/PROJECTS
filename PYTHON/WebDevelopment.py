# A Django-based e-commerce application for a drone store with a shopping cart and checkout system

import os
import django
from django.db import models
from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.urls import path
from django.conf import settings

# Setting up Django environment and settings for an in-memory SQLite database.
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "dronestore.settings")
django.setup()

# Django Settings
settings.configure(
    INSTALLED_APPS=[
        'django.contrib.contenttypes',  # Required for models to work
        'django.contrib.sessions',
    ],
    DATABASES={
        'default': {
            'ENGINE': 'django.db.backends.sqlite3',
            'NAME': 'dronestore_db.sqlite3',
        }
    },
)

# Defining models for the drone store application

class Category(models.Model):
    name = models.CharField(max_length=100)

    def __str__(self):
        return self.name


class Product(models.Model):
    name = models.CharField(max_length=200)
    description = models.TextField()
    price = models.DecimalField(max_digits=10, decimal_places=2)
    stock_quantity = models.IntegerField()
    category = models.ForeignKey(Category, on_delete=models.CASCADE)

    def __str__(self):
        return self.name


class Order(models.Model):
    user = models.CharField(max_length=100)
    total_amount = models.DecimalField(max_digits=10, decimal_places=2)
    status = models.CharField(max_length=20, default='Pending')

    def __str__(self):
        return f"Order {self.id} by {self.user}"

# Views for the drone store application

def product_list(request):
    products = Product.objects.all()
    return render(request, 'product_list.html', {'products': products})


def add_to_cart(request, product_id):
    product = Product.objects.get(id=product_id)
    cart = request.session.get('cart', {})
    
    if product_id in cart:
        cart[product_id]['quantity'] += 1
    else:
        cart[product_id] = {'name': product.name, 'price': str(product.price), 'quantity': 1}

    request.session['cart'] = cart
    return redirect('product_list')


def cart(request):
    cart = request.session.get('cart', {})
    total_amount = sum(float(item['price']) * item['quantity'] for item in cart.values())
    return render(request, 'cart.html', {'cart': cart, 'total_amount': total_amount})


def checkout(request):
    if request.method == 'POST':
        cart = request.session.get('cart', {})
        if not cart:
            return HttpResponse("Your cart is empty.", status=400)
        
        total_amount = sum(float(item['price']) * item['quantity'] for item in cart.values())
        order = Order.objects.create(user=request.user.username, total_amount=total_amount)

        # Simulate payment gateway interaction (mocked as success)
        order.status = 'Completed'
        order.save()

        # Clear the cart
        request.session['cart'] = {}
        return redirect('order_success')

    return render(request, 'checkout.html')


def order_success(request):
    return HttpResponse("Order successfully placed.")


# Define URLs for the views
urlpatterns = [
    path('', product_list, name='product_list'),
    path('add_to_cart/<int:product_id>/', add_to_cart, name='add_to_cart'),
    path('cart/', cart, name='cart'),
    path('checkout/', checkout, name='checkout'),
    path('order_success/', order_success, name='order_success'),
]

# Templates (simplified for this example)

# HTML Template for Product List (to be used in product_list.html)
product_list_html = """
<!DOCTYPE html>
<html>
<head>
    <title>Drone Store - Product List</title>
</head>
<body>
    <h1>Drone Products</h1>
    <ul>
        {% for product in products %}
            <li>
                {{ product.name }} - ${{ product.price }} 
                <a href="{% url 'add_to_cart' product.id %}">Add to Cart</a>
            </li>
        {% endfor %}
    </ul>
</body>
</html>
"""

# HTML Template for Cart (to be used in cart.html)
cart_html = """
<!DOCTYPE html>
<html>
<head>
    <title>Shopping Cart</title>
</head>
<body>
    <h1>Your Shopping Cart</h1>
    <ul>
        {% for item in cart.values %}
            <li>{{ item.name }} - ${{ item.price }} x {{ item.quantity }}</li>
        {% endfor %}
    </ul>
    <h2>Total: ${{ total_amount }}</h2>
    <form method="POST" action="{% url 'checkout' %}">
        {% csrf_token %}
        <button type="submit">Proceed to Checkout</button>
    </form>
</body>
</html>
"""

# HTML Template for Checkout (to be used in checkout.html)
checkout_html = """
<!DOCTYPE html>
<html>
<head>
    <title>Checkout</title>
</head>
<body>
    <h1>Checkout</h1>
    <form method="POST">
        {% csrf_token %}
        <p>Your total is: ${{ total_amount }}</p>
        <button type="submit">Place Order</button>
    </form>
</body>
</html>
"""

# Define a simple Django request handler for rendering templates.
from django.template import Template, Context
from django.http import HttpResponse

def render_template(request, template_str, context=None):
    context = context or {}
    template = Template(template_str)
    return HttpResponse(template.render(Context(context)))

# Connect views to HTML templates
def product_list(request):
    products = Product.objects.all()
    return render_template(request, product_list_html, {'products': products})


def cart(request):
    cart = request.session.get('cart', {})
    total_amount = sum(float(item['price']) * item['quantity'] for item in cart.values())
    return render_template(request, cart_html, {'cart': cart, 'total_amount': total_amount})


def checkout(request):
    if request.method == 'POST':
        cart = request.session.get('cart', {})
        if not cart:
            return HttpResponse("Your cart is empty.", status=400)
        
        total_amount = sum(float(item['price']) * item['quantity'] for item in cart.values())
        order = Order.objects.create(user=request.user.username, total_amount=total_amount)

        # Simulate payment gateway interaction (mocked as success)
        order.status = 'Completed'
        order.save()

        # Clear the cart
        request.session['cart'] = {}
        return redirect('order_success')

    return render_template(request, checkout_html)


# Run the Django application
if __name__ == '__main__':
    from django.core.management import execute_from_command_line

    # Create tables in the database
    os.environ['DJANGO_SETTINGS_MODULE'] = 'dronestore.settings'
    execute_from_command_line(['manage.py', 'migrate'])

    # Start the server
    execute_from_command_line(['manage.py', 'runserver'])