pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('Build') {
      steps {
        sh '''cd build;cmake ..;make;'''
      }
    }

    stage('Test') {
      steps {
        sh './runTest'
      }
    }

  }
}