pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile'
    }

  }
  stages {
    stage('Build') {
      steps {
        sh 'cd build'
        sh 'cmake ..'
        sh 'make'
      }
    }

    stage('Test') {
      steps {
        sh './runTest'
      }
    }

  }
}