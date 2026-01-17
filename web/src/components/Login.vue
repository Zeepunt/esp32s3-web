<script setup lang="ts">
import { ref, computed } from 'vue'
import axios from 'axios'

const username = ref('')
const password = ref('')
const error = ref('')
const loading = ref(false)
const language = ref('zh') // 'zh' for Chinese, 'en' for English

// 中英文文本映射
const translations = {
  zh: {
    title: '后台管理',
    username: '用户名:',
    password: '密码:',
    placeholderUsername: '请输入用户名',
    placeholderPassword: '请输入密码',
    login: '登录',
    error: '请输入用户名和密码',
    success: '登录成功！',
    switchToEnglish: 'English',
    switchToChinese: '中文',
    loginFailed: '登录失败，请检查用户名和密码',
    networkError: '网络错误，请稍后重试'
  },
  en: {
    title: 'Admin Panel',
    username: 'Username:',
    password: 'Password:',
    placeholderUsername: 'Enter username',
    placeholderPassword: 'Enter password',
    login: 'Login',
    error: 'Please enter username and password',
    success: 'Login successful!',
    switchToChinese: '中文',
    switchToEnglish: 'English',
    loginFailed: 'Login failed, please check username and password',
    networkError: 'Network error, please try again later'
  }
}

// 获取当前语言的文本
const t = computed(() => translations[language.value as keyof typeof translations])

// 切换语言
const toggleLanguage = () => {
  language.value = language.value === 'zh' ? 'en' : 'zh'
}

const handleLogin = async (e: Event) => {
  e.preventDefault()
  error.value = ''
  loading.value = true
  
  if (!username.value || !password.value) {
    error.value = t.value.error
    loading.value = false
    return
  }
  
  try {
    // 发送POST请求到后台接口
    const response = await axios.post('/system/login', {
      username: username.value,
      password: password.value
    })
    
    // 假设后台返回成功信息
    if (response.status === 200) {
      alert(t.value.success)
      // 登录成功后可以跳转到其他页面
      console.log('登录成功，返回数据:', response.data)
    }
  } catch (err) {
    if (axios.isAxiosError(err)) {
      if (err.response?.status === 401) {
        error.value = t.value.loginFailed
      } else {
        error.value = t.value.networkError
      }
    } else {
      error.value = t.value.networkError
    }
    console.error('登录错误:', err)
  } finally {
    loading.value = false
  }
}
</script>

<template>
  <div class="login-container">
    <div class="login-window">
      <div class="language-switch">
        <button @click="toggleLanguage" class="language-button">
          {{ language === 'zh' ? t.switchToEnglish : t.switchToChinese }}
        </button>
      </div>
      
      <h2>{{ t.title }}</h2>
      
      <form @submit="handleLogin" class="login-form">
        <div class="form-group">
          <label for="username">{{ t.username }}</label>
          <input 
            id="username" 
            v-model="username" 
            type="text" 
            :placeholder="t.placeholderUsername"
            required
            :disabled="loading"
          />
        </div>
        
        <div class="form-group">
          <label for="password">{{ t.password }}</label>
          <input 
            id="password" 
            v-model="password" 
            type="password" 
            :placeholder="t.placeholderPassword"
            required
            :disabled="loading"
          />
        </div>
        
        <div v-if="error" class="error-message">
          {{ error }}
        </div>
        
        <button type="submit" class="login-button" :disabled="loading">
          {{ loading ? '登录中...' : t.login }}
        </button>
      </form>
    </div>
  </div>
</template>

<style scoped>
* {
  box-sizing: border-box;
}

.login-container {
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 100vh;
  width: 100vw;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  padding: 1rem;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
  margin: 0;
  overflow-x: hidden;
}

.login-window {
  background: white;
  border-radius: 8px;
  box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);
  padding: 2rem;
  width: 100%;
  max-width: 90%;
  min-width: 300px;
  max-width: 700px;
  position: relative;
}

.language-switch {
  position: absolute;
  top: 1rem;
  right: 1rem;
}

.language-button {
  background: transparent;
  border: 1px solid #667eea;
  color: #667eea;
  padding: 0.25rem 0.75rem;
  border-radius: 4px;
  font-size: 0.875rem;
  cursor: pointer;
  transition: all 0.2s;
  -webkit-appearance: none;
  -moz-appearance: none;
  appearance: none;
}

.language-button:hover {
  background-color: #667eea;
  color: white;
}

.login-window h2 {
  margin: 0 0 1.5rem;
  color: #333;
  text-align: center;
  font-size: 1.5rem;
}

.login-form {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.form-group {
  display: flex;
  align-items: center;
  gap: 1rem;
  margin-bottom: 1rem;
}

.form-group label {
  font-weight: 500;
  color: #555;
  font-size: 1rem;
  width: 80px;
  text-align: right;
}

.form-group input {
  padding: 0.75rem;
  border: 1px solid #ddd;
  border-radius: 4px;
  font-size: 1rem;
  transition: border-color 0.2s, box-shadow 0.2s;
  flex: 1;
  -webkit-appearance: none;
  -moz-appearance: none;
  appearance: none;
}

.form-group input:focus {
  outline: none;
  border-color: #667eea;
  box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2);
}

.error-message {
  color: #dc3545;
  font-size: 0.875rem;
  margin-top: -0.5rem;
}

.login-button {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  padding: 0.75rem 2rem;
  border-radius: 4px;
  font-size: 1rem;
  font-weight: 500;
  cursor: pointer;
  transition: opacity 0.2s, transform 0.1s;
  margin-top: 1rem;
  margin-left: auto;
  margin-right: auto;
  display: block;
  -webkit-appearance: none;
  -moz-appearance: none;
  appearance: none;
}

.login-button:hover {
  opacity: 0.9;
}

.login-button:active {
  transform: translateY(1px);
}

/* 移除了不再使用的login-footer样式 */

/* 响应式设计 */
@media (max-width: 576px) {
  .login-window {
    padding: 1.5rem;
    min-width: 280px;
  }
  
  .login-window h2 {
    font-size: 1.3rem;
  }
  
  .form-group {
    flex-direction: column;
    align-items: stretch;
    gap: 0.5rem;
  }
  
  .form-group label {
    width: auto;
    text-align: left;
  }
  
  .form-group input {
    padding: 0.6rem;
    font-size: 0.9rem;
  }
  
  .login-button {
    padding: 0.6rem 1.5rem;
    font-size: 0.9rem;
  }
}

@media (max-height: 500px) {
  .login-container {
    min-height: auto;
    height: 100vh;
  }
  
  .login-window {
    padding: 1rem;
  }
}
</style>