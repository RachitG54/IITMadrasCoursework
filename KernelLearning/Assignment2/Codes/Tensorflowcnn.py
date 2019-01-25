'''
A Convolutional Network implementation example using TensorFlow library.
This example is using the MNIST database of handwritten digits
(http://yann.lecun.com/exdb/mnist/)

Author: Aymeric Damien
Project: https://github.com/aymericdamien/TensorFlow-Examples/
'''

from __future__ import print_function

import tensorflow as tf
import scipy.io as spio
import numpy as np; 
import math
import random
import cPickle
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from PIL import Image

def loadmat(filename):
    '''
    this function should be called instead of direct spio.loadmat
    as it cures the problem of not properly recovering python dictionaries
    from mat files. It calls the function check keys to cure all entries
    which are still mat-objects
    '''
    data = spio.loadmat(filename, struct_as_record=False, squeeze_me=True)
    return _check_keys(data)

def _check_keys(dict):
    '''
    checks if entries in dictionary are mat-objects. If yes
    todict is called to change them to nested dictionaries
    '''
    for key in dict:
        if isinstance(dict[key], spio.matlab.mio5_params.mat_struct):
            dict[key] = _todict(dict[key])
    return dict        

def _todict(matobj):
    '''
    A recursive function which constructs from matobjects nested dictionaries
    '''
    dict = {}
    for strg in matobj._fieldnames:
        elem = matobj.__dict__[strg]
        if isinstance(elem, spio.matlab.mio5_params.mat_struct):
            dict[strg] = _todict(elem)
        else:
            dict[strg] = elem
    return dict

def diff(first, second):
        second = set(second)
        return [item for item in first if item not in second]

def get_actual_data():
    data = []
    label = []
    for i in range(total_label.size):
        if( total_label[i] == 40 ):
            data.append(total_data[i])
            label.append(0)
        elif( total_label[i] == 48 ):
            data.append(total_data[i])
            label.append(1)
        elif( total_label[i] == 56 ):
            data.append(total_data[i])
            label.append(2)
        elif( total_label[i] == 76 ):
            data.append(total_data[i])
            label.append(3)
        elif( total_label[i] == 93 ):
            data.append(total_data[i])
            label.append(4)
    return (data,label)

def split_data():
    split_indices = random.sample(range(467),281)
    train_data = []
    test_data = []
    train_label = []
    test_label = []
    val_data = []
    val_label = []  
    for i in split_indices:
        train_data.append(data[i])
        temp = np.zeros(5)
        temp[int(label[i])] = 1
        train_label.append(temp)
    train = (np.array(train_data),np.array(train_label))
    comb_data = []
    comb_label = []
    for i in diff(range(467),split_indices):
        comb_data.append(data[i])
        temp = np.zeros(5)
        temp[int(label[i])] = 1
        comb_label.append(temp)
        
    spl_indices = random.sample(range(186),84)
    for i in spl_indices:
        val_data.append(comb_data[i])
        val_label.append(comb_label[i])
    
    for i in diff(range(186),spl_indices):
        test_data.append(comb_data[i])
        test_label.append(comb_label[i])
        
    train = (np.array(train_data),np.array(train_label))
    val = (np.array(val_data),np.array(val_label))          
    test = (np.array(test_data),np.array(test_label))
    return (train,val,test)


mat = loadmat('2.Blackandwhiteimagedata/binaryData.mat')
total_data = mat['data']['data']
total_label = mat['data']['labels']
(data,label) = get_actual_data()
(train_data,valid_data,test_data) = split_data()

print(test_data[1])

# Parameters

learning_rate = 0.001
training_iters = 200
display_step = 10

# Network Parameters
n_input = 784 # MNIST data input (img shape: 28*28)
n_classes = 5 # MNIST total classes (0-9 digits)
dropout = 0.75 # Dropout, probability to keep units

# tf Graph input
x = tf.placeholder(tf.float32, [None, n_input])
y = tf.placeholder(tf.float32, [None, n_classes])
keep_prob = tf.placeholder(tf.float32) #dropout (keep probability)


# Create some wrappers for simplicity
def conv2d(x, W, b, strides=1):
    # Conv2D wrapper, with bias and relu activation
    x = tf.nn.conv2d(x, W, strides=[1, strides, strides, 1], padding='SAME')
    x = tf.nn.bias_add(x, b)
    return tf.nn.relu(x)


def maxpool2d(x, k=2):
    # MaxPool2D wrapper
    return tf.nn.max_pool(x, ksize=[1, k, k, 1], strides=[1, k, k, 1],
                          padding='SAME')


# Create model
def conv_net(x, weights, biases, dropout):
    # Reshape input picture
    x = tf.reshape(x, shape=[-1, 28, 28, 1])

    # Convolution Layer
    conv1 = conv2d(x, weights['wc1'], biases['bc1'])
    # Max Pooling (down-sampling)
    conv1 = maxpool2d(conv1, k=2)

    # Convolution Layer
    conv2 = conv2d(conv1, weights['wc2'], biases['bc2'])
    # Max Pooling (down-sampling)
    conv2 = maxpool2d(conv2, k=2)

    # Fully connected layer
    # Reshape conv2 output to fit fully connected layer input
    fc1 = tf.reshape(conv2, [-1, weights['wd1'].get_shape().as_list()[0]])
    fc1 = tf.add(tf.matmul(fc1, weights['wd1']), biases['bd1'])
    fc1 = tf.nn.relu(fc1)
    # Apply Dropout
    fc1 = tf.nn.dropout(fc1, dropout)

    # Output, class prediction
    out = tf.add(tf.matmul(fc1, weights['out']), biases['out'])
    return out

# Store layers weight & bias
weights = {
    # 5x5 conv, 1 input, 32 outputs
    'wc1': tf.Variable(tf.random_normal([5, 5, 1, 10])),
    # 5x5 conv, 32 inputs, 64 outputs
    'wc2': tf.Variable(tf.random_normal([5, 5, 10, 20])),
    # fully connected, 7*7*64 inputs, 1024 outputs
    'wd1': tf.Variable(tf.random_normal([7*7*20, 1024])),
    # 1024 inputs, 10 outputs (class prediction)
    'out': tf.Variable(tf.random_normal([1024, n_classes]))
}

biases = {
    'bc1': tf.Variable(tf.random_normal([10])),
    'bc2': tf.Variable(tf.random_normal([20])),
    'bd1': tf.Variable(tf.random_normal([1024])),
    'out': tf.Variable(tf.random_normal([n_classes]))
}

# Construct model
pred = conv_net(x, weights, biases, keep_prob)

# Define loss and optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# Evaluate model
correct_pred = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

prediction = tf.cast(tf.argmax(pred, 1), tf.float32)

# Initializing the variables
init = tf.global_variables_initializer()

# Launch the graph
prevacc = -1;
curracc = 0;
with tf.Session() as sess:
    sess.run(init)
    step = 1
    # Keep training until reach max iterations
    while step < training_iters:
        # Run optimization op (backprop)
        sess.run(optimizer, feed_dict={x: train_data[0], y: train_data[1],
                                       keep_prob: dropout})
        if step % display_step == 0:
            # Calculate batch loss and accuracy
            loss, acc = sess.run([cost, accuracy], feed_dict={x: train_data[0], y: train_data[1],keep_prob: 1.})
            print("Iter " + str(step) + ",Loss= " + "{:.6f}".format(loss) + ", Training Accuracy= " + "{:.5f}".format(acc))
            prevacc = curracc;
            curracc = acc;

        step += 1

    print("Optimization Finished!")

    # Calculate accuracy for 256 mnist test images
    print("Testing Accuracy:", \
        sess.run(accuracy, feed_dict={x:valid_data[0], y: valid_data[1],keep_prob: 1.}))

    predictionsess = sess.run(prediction, feed_dict={x: test_data[0], y: test_data[1],keep_prob: 1.})
    print("{:s}".format(predictionsess))