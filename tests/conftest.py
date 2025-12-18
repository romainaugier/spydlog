import pytest
import spydlog


@pytest.fixture(autouse=True)
def cleanup_loggers():
    """Cleanup loggers after each test to avoid conflicts"""
    yield
    # Drop all loggers after each test and reset the default
    spydlog.drop_all()
    default = spydlog.logger("")
    spydlog.set_default_logger(default)


@pytest.fixture(autouse=True)
def reset_global_settings():
    """Reset global logger settings after each test"""
    yield
    # Reset to default settings
    spydlog.set_level(spydlog.level.info)
    spydlog.set_pattern("%+")  # Default pattern
    spydlog.flush_on(spydlog.level.off)


def pytest_configure(config):
    """Configure pytest with custom markers"""
    config.addinivalue_line(
        "markers", "slow: marks tests as slow (deselect with '-m \"not slow\"')"
    )
    config.addinivalue_line(
        "markers", "async_test: marks tests that use async logging"
    )
