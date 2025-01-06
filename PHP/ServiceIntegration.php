<?php

// A custom WordPress plugin that integrates an online payment platform and manages e-commerce transactions.

// Prevent direct access to the script
if (!defined('ABSPATH')) {
    exit;
}

// Step 1: Register Custom Post Type for Products
function wpec_register_product_post_type() {
    register_post_type('wpec_product', array(
        'labels' => array(
            'name' => __('Products', 'wpec'),
            'singular_name' => __('Product', 'wpec'),
            'add_new' => __('Add New Product', 'wpec'),
        ),
        'public' => true,
        'has_archive' => true,
        'supports' => array('title', 'editor', 'thumbnail'),
    ));
}
add_action('init', 'wpec_register_product_post_type');

// Step 2: Create Custom Table for Transactions
function wpec_create_transaction_table() {
    global $wpdb;
    $table_name = $wpdb->prefix . 'wpec_transactions';
    $charset_collate = $wpdb->get_charset_collate();

    $sql = "CREATE TABLE $table_name (
        id INT AUTO_INCREMENT PRIMARY KEY,
        product_id INT NOT NULL,
        user_id INT NOT NULL,
        amount DECIMAL(10, 2) NOT NULL,
        status VARCHAR(50) NOT NULL,
        created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (product_id) REFERENCES {$wpdb->prefix}posts(ID),
        FOREIGN KEY (user_id) REFERENCES {$wpdb->prefix}users(ID)
    ) $charset_collate;";

    require_once(ABSPATH . 'wp-admin/includes/upgrade.php');
    dbDelta($sql);
}
register_activation_hook(__FILE__, 'wpec_create_transaction_table');

// Step 3: Add Payment Form to Product Pages
function wpec_add_payment_form($content) {
    if (is_singular('wpec_product')) {
        $product_id = get_the_ID();
        $nonce = wp_create_nonce('wpec_payment_form'); // Add nonce for security
        $content .= '<form action="' . esc_url(admin_url('admin-post.php')) . '" method="POST">';
        $content .= '<input type="hidden" name="action" value="wpec_process_payment">';
        $content .= '<input type="hidden" name="product_id" value="' . esc_attr($product_id) . '">';
        $content .= '<input type="hidden" name="wpec_payment_nonce" value="' . esc_attr($nonce) . '">'; // Include nonce
        $content .= '<label for="wpec_amount">Amount:</label>';
        $content .= '<input type="number" name="amount" id="wpec_amount" step="0.01" required>';
        $content .= '<button type="submit">Pay Now</button>';
        $content .= '</form>';
    }
    return $content;
}
add_filter('the_content', 'wpec_add_payment_form');

// Step 4: Handle Payment Submission
function wpec_process_payment() {
    // Verify nonce for security
    if (!isset($_POST['wpec_payment_nonce']) || !wp_verify_nonce($_POST['wpec_payment_nonce'], 'wpec_payment_form')) {
        wp_die('Nonce verification failed.');
    }

    if (!isset($_POST['product_id'], $_POST['amount'])) {
        wp_die('Invalid request');
    }

    global $wpdb;
    $product_id = intval($_POST['product_id']);
    $user_id = get_current_user_id();
    $amount = floatval($_POST['amount']);
    $status = 'Pending'; // Initial status

    // Check if product and user exist
    if (!get_post($product_id) || !get_userdata($user_id)) {
        wp_die('Invalid product or user.');
    }

    // Simulated payment gateway response
    $payment_successful = ($amount > 0);

    if ($payment_successful) {
        $status = 'Completed';
    } else {
        $status = 'Failed';
    }

    // Insert transaction into database
    $wpdb->insert($wpdb->prefix . 'wpec_transactions', array(
        'product_id' => $product_id,
        'user_id' => $user_id,
        'amount' => $amount,
        'status' => $status,
    ));

    // Redirect to payment status page
    wp_redirect(home_url('/payment-status/?status=' . $status));
    exit;
}
add_action('admin_post_wpec_process_payment', 'wpec_process_payment');
add_action('admin_post_nopriv_wpec_process_payment', 'wpec_process_payment');

// Step 5: Display Payment Status
function wpec_payment_status_page() {
    if (isset($_GET['status'])) {
        $status = sanitize_text_field($_GET['status']);
        return '<h2>Payment Status: ' . esc_html($status) . '</h2>';
    }
}
add_shortcode('wpec_payment_status', 'wpec_payment_status_page');

// Step 6: Add Admin Menu for Viewing Transactions
function wpec_add_admin_menu() {
    add_menu_page(
        'E-Commerce Transactions',
        'Transactions',
        'manage_options',
        'wpec-transactions',
        'wpec_display_transactions',
        'dashicons-cart',
        20
    );
}
add_action('admin_menu', 'wpec_add_admin_menu');

function wpec_display_transactions() {
    global $wpdb;
    $table_name = $wpdb->prefix . 'wpec_transactions';
    $results = $wpdb->get_results("SELECT * FROM $table_name");

    echo '<h1>Transactions</h1>';
    echo '<table>';
    echo '<tr><th>ID</th><th>Product</th><th>User</th><th>Amount</th><th>Status</th><th>Date</th></tr>';
    foreach ($results as $row) {
        echo '<tr>';
        echo '<td>' . esc_html($row->id) . '</td>';
        echo '<td>' . esc_html(get_the_title($row->product_id)) . '</td>';
        echo '<td>' . esc_html(get_userdata($row->user_id)->user_login) . '</td>';
        echo '<td>' . esc_html($row->amount) . '</td>';
        echo '<td>' . esc_html($row->status) . '</td>';
        echo '<td>' . esc_html($row->created_at) . '</td>';
        echo '</tr>';
    }
    echo '</table>';
}