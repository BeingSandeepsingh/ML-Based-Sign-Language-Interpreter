

# %%
import os
import numpy as np
from sklearn import svm
from sklearn import tree
from sklearn.ensemble import RandomForestClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.metrics import confusion_matrix



import pandas as pd
path=r"E:\Sign langugae recognition\ASL-Sensor-Dataglove-Dataset - Copy\001.xlsx"
df=pd.read_excel(path)
df.head(5)

# %%
df.shape

# %%
df.info(memory_usage='deep')

# %%
df.columns

# %%
df=pd.read_excel(path,usecols=['Source.Name','flex_1','flex_2','flex_3','flex_4','flex_5','ACCx',
       'ACCy', 'ACCz'])
df.shape


# %%
df['target'] = df['Source.Name'].apply(lambda x: os.path.splitext(x)[0])
df.head()

# %%
from sklearn.model_selection import train_test_split
train, test = train_test_split(df, test_size = 0.2)

# Extract features and labels
train_features = train[['flex_1', 'flex_2', 'flex_3', 'flex_4', 'flex_5', 'ACCx', 'ACCy', 'ACCz']]
train_label = train['target']

test_features = test[['flex_1', 'flex_2', 'flex_3', 'flex_4', 'flex_5', 'ACCx', 'ACCy', 'ACCz']]
test_label = test['target']

# %%
model = svm.SVC(kernel='linear', gamma=1, C=1)
model.fit(train_features, train_label)
# Model evaluation
print("Results for combined dataset:")
   
# Training accuracy
train_accuracy = model.score(train_features, train_label)
print("Training Accuracy:", train_accuracy)
# Test predictions
predicted_svm = model.predict(test_features)
    
# Test accuracy
test_accuracy = accuracy_score(test_label, predicted_svm)
print("Test Accuracy:", test_accuracy)
# Confusion matrix
cn = confusion_matrix(test_label, predicted_svm)
print("Confusion Matrix:")
print(cn)

# %%
# Assuming 'model' is your trained SVM model
input_data = [[-9,5,-8,-9,-7,5.916846,7.887134,2.525366]]

prediction = model.predict(input_data)
print("Model Prediction:", prediction)


